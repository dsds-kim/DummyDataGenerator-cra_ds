#include "DataGenerator.h"
#include <chrono>
#include <iomanip>
#include <sstream>

std::wstring DataGenerator::generateString(int maxLength)
{
    static const std::wstring k_chars =
        L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    maxLength = std::max(1, maxLength);

    std::uniform_int_distribution<int> lenDist(1, maxLength);
    std::uniform_int_distribution<int> charDist(0, static_cast<int>(k_chars.size()) - 1);

    int len = lenDist(m_rng);
    std::wstring result;
    result.reserve(len);
    for (int i = 0; i < len; ++i)
        result += k_chars[charDist(m_rng)];

    return result;
}

int DataGenerator::generateInt(int min, int max)
{
    return std::uniform_int_distribution<int>{ min, max }(m_rng);
}

double DataGenerator::generateDouble(double min, double max)
{
    return std::uniform_real_distribution<double>{ min, max }(m_rng);
}

std::wstring DataGenerator::generateDate(const std::wstring& startDate, const std::wstring& endDate)
{
    auto parseDate = [](const std::wstring& s) {
        int y = std::stoi(s.substr(0, 4));
        int m = std::stoi(s.substr(5, 2));
        int d = std::stoi(s.substr(8, 2));
        return std::chrono::sys_days{
            std::chrono::year_month_day{
                std::chrono::year{ y },
                std::chrono::month{ static_cast<unsigned>(m) },
                std::chrono::day{  static_cast<unsigned>(d) }
            }
        };
    };

    auto start = parseDate(startDate);
    auto end   = parseDate(endDate);
    int  range = static_cast<int>((end - start).count());

    auto picked = start + std::chrono::days{ std::uniform_int_distribution<int>{ 0, range }(m_rng) };
    auto ymd    = std::chrono::year_month_day{ picked };

    std::wostringstream oss;
    oss << std::setfill(L'0')
        << std::setw(4) << static_cast<int>(ymd.year())  << L'-'
        << std::setw(2) << static_cast<unsigned>(ymd.month()) << L'-'
        << std::setw(2) << static_cast<unsigned>(ymd.day());
    return oss.str();
}

std::wstring DataGenerator::generateEmail()
{
    static const std::vector<std::wstring> k_domains = {
        L"example.com", L"test.com", L"mail.com", L"sample.org"
    };

    auto localPart = generateString(10);
    int  idx       = std::uniform_int_distribution<int>{ 0, static_cast<int>(k_domains.size()) - 1 }(m_rng);

    return localPart + L"@" + k_domains[idx];
}

std::wstring DataGenerator::buildInsertSql(const std::wstring& table, const std::vector<ColumnDef>& columns)
{
    std::wstring colList, valList;

    for (size_t i = 0; i < columns.size(); ++i)
    {
        if (i > 0) { colList += L", "; valList += L", "; }

        const auto& col = columns[i];
        colList += col.name;

        switch (col.type)
        {
        case ColumnDef::Type::STRING:
            valList += L"'" + generateString(col.maxLength) + L"'";
            break;
        case ColumnDef::Type::INT:
            valList += std::to_wstring(generateInt(col.intMin, col.intMax));
            break;
        case ColumnDef::Type::DOUBLE:
        {
            std::wostringstream oss;
            oss << std::fixed << std::setprecision(2) << generateDouble(col.dblMin, col.dblMax);
            valList += oss.str();
            break;
        }
        case ColumnDef::Type::DATE:
            valList += L"'" + generateDate(col.dateStart, col.dateEnd) + L"'";
            break;
        case ColumnDef::Type::EMAIL:
            valList += L"'" + generateEmail() + L"'";
            break;
        }
    }

    return L"INSERT INTO " + table + L" (" + colList + L") VALUES (" + valList + L")";
}
