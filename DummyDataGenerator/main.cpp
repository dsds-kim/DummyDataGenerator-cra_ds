#include "DataGenerator.h"
#include "DbConnection.h"
#include <iostream>

int main()
{
    // Adjust connection string to match your environment.
    // SQL Server : L"DRIVER={ODBC Driver 18 for SQL Server};SERVER=localhost;DATABASE=TestDB;UID=sa;PWD=password;"
    // MySQL      : L"DRIVER={MySQL ODBC 8.0 Driver};SERVER=localhost;DATABASE=TestDB;UID=root;PWD=password;"
    // PostgreSQL : L"DRIVER={PostgreSQL Unicode};SERVER=localhost;DATABASE=TestDB;UID=postgres;PWD=password;"
    const std::wstring connStr =
        L"DRIVER={ODBC Driver 18 for SQL Server};"
        L"SERVER=localhost;"
        L"DATABASE=TestDB;"
        L"UID=sa;"
        L"PWD=password;";

    const std::wstring targetTable = L"Users";
    const int          rowCount    = 10;

    // Schema: define target table columns and their generation rules.
    const std::vector<ColumnDef> schema = {
        { .name = L"Name",      .type = ColumnDef::Type::STRING, .maxLength = 50                                     },
        { .name = L"Age",       .type = ColumnDef::Type::INT,    .intMin = 18,        .intMax = 80                   },
        { .name = L"Score",     .type = ColumnDef::Type::DOUBLE, .dblMin = 0.0,       .dblMax = 100.0               },
        { .name = L"BirthDate", .type = ColumnDef::Type::DATE,   .dateStart = L"1944-01-01", .dateEnd = L"2006-12-31" },
        { .name = L"Email",     .type = ColumnDef::Type::EMAIL                                                       },
    };

    try
    {
        DbConnection  db(connStr);
        DataGenerator gen;

        db.connect();
        std::cout << "DB connection successful\n";

        for (int i = 0; i < rowCount; ++i)
        {
            auto sql = gen.buildInsertSql(targetTable, schema);
            db.executeNonQuery(sql);
        }

        std::wcout << rowCount << L" rows inserted into " << targetTable << L"\n";
        db.disconnect();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
