#include "DataGenerator.h"
#include "DataGeneratorTest.h"
#include "TestRunner.h"

void registerDataGeneratorTests(TestRunner& runner)
{
    runner.add("string_length_within_max", [] {
        DataGenerator gen;
        for (int i = 0; i < 100; ++i)
        {
            auto s = gen.generateString(10);
            ASSERT_TRUE(s.size() <= 10);
        }
    });

    runner.add("int_within_range", [] {
        DataGenerator gen;
        for (int i = 0; i < 100; ++i)
        {
            int v = gen.generateInt(5, 20);
            ASSERT_TRUE(v >= 5 && v <= 20);
        }
    });

    runner.add("double_within_range", [] {
        DataGenerator gen;
        for (int i = 0; i < 100; ++i)
        {
            double v = gen.generateDouble(1.0, 10.0);
            ASSERT_TRUE(v >= 1.0 && v <= 10.0);
        }
    });

    runner.add("date_format_valid", [] {
        DataGenerator gen;
        auto date = gen.generateDate(L"2020-01-01", L"2023-12-31");
        // YYYY-MM-DD: length == 10, dashes at positions 4 and 7
        ASSERT_TRUE(date.size() == 10);
        ASSERT_TRUE(date[4] == L'-' && date[7] == L'-');
    });

    runner.add("date_within_range", [] {
        DataGenerator gen;
        for (int i = 0; i < 100; ++i)
        {
            auto date = gen.generateDate(L"2020-01-01", L"2020-12-31");
            ASSERT_TRUE(date >= L"2020-01-01" && date <= L"2020-12-31");
        }
    });

    runner.add("email_format_valid", [] {
        DataGenerator gen;
        for (int i = 0; i < 20; ++i)
        {
            auto email = gen.generateEmail();
            auto at = email.find(L'@');
            ASSERT_TRUE(at != std::wstring::npos); // '@' exists
            ASSERT_TRUE(at > 0);                   // something before '@'
            auto dot = email.find(L'.', at);
            ASSERT_TRUE(dot != std::wstring::npos); // domain has '.'
            ASSERT_TRUE(dot < email.size() - 1);    // something after '.'
        }
    });

    runner.add("build_insert_sql_shape", [] {
        DataGenerator gen;
        std::vector<ColumnDef> cols = {
            { .name = L"Name", .type = ColumnDef::Type::STRING, .maxLength = 20 },
            { .name = L"Age",  .type = ColumnDef::Type::INT,    .intMin = 18, .intMax = 65 },
        };
        auto sql = gen.buildInsertSql(L"Users", cols);

        ASSERT_TRUE(sql.find(L"INSERT INTO Users") == 0);
        ASSERT_TRUE(sql.find(L"Name")   != std::wstring::npos);
        ASSERT_TRUE(sql.find(L"Age")    != std::wstring::npos);
        ASSERT_TRUE(sql.find(L"VALUES") != std::wstring::npos);
        ASSERT_TRUE(sql.back() == L')');
    });
}
