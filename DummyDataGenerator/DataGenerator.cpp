#include "DataGenerator.h"
#include <stdexcept>

static constexpr auto k_notImplemented = "not implemented";

std::wstring DataGenerator::generateString(int)
{
    throw std::runtime_error(k_notImplemented);
}

int DataGenerator::generateInt(int, int)
{
    throw std::runtime_error(k_notImplemented);
}

double DataGenerator::generateDouble(double, double)
{
    throw std::runtime_error(k_notImplemented);
}

std::wstring DataGenerator::generateDate(const std::wstring&, const std::wstring&)
{
    throw std::runtime_error(k_notImplemented);
}

std::wstring DataGenerator::generateEmail()
{
    throw std::runtime_error(k_notImplemented);
}

std::wstring DataGenerator::buildInsertSql(const std::wstring&, const std::vector<ColumnDef>&)
{
    throw std::runtime_error(k_notImplemented);
}
