#pragma once
#include <string>
#include <vector>

struct ColumnDef
{
    enum class Type { STRING, INT, DOUBLE, DATE, EMAIL };

    std::wstring name;
    Type         type        = Type::STRING;
    int          maxLength   = 50;               // STRING
    int          intMin      = 0;                // INT
    int          intMax      = 100;              // INT
    double       dblMin      = 0.0;              // DOUBLE
    double       dblMax      = 100.0;            // DOUBLE
    std::wstring dateStart   = L"2000-01-01";    // DATE
    std::wstring dateEnd     = L"2024-12-31";    // DATE
};

class DataGenerator
{
public:
    std::wstring generateString(int maxLength);
    int          generateInt   (int min, int max);
    double       generateDouble(double min, double max);
    std::wstring generateDate  (const std::wstring& startDate, const std::wstring& endDate);
    std::wstring generateEmail ();

    // Generates a random value per column and returns a complete INSERT statement.
    std::wstring buildInsertSql(const std::wstring& table, const std::vector<ColumnDef>& columns);
};
