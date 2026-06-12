#include "DbConnection.h"
#include <iostream>

int main()
{
    // 연결 문자열 예시 (환경에 맞게 수정)
    // SQL Server : L"DRIVER={ODBC Driver 18 for SQL Server};SERVER=localhost;DATABASE=TestDB;UID=sa;PWD=password;"
    // MySQL      : L"DRIVER={MySQL ODBC 8.0 Driver};SERVER=localhost;DATABASE=TestDB;UID=root;PWD=password;"
    // PostgreSQL : L"DRIVER={PostgreSQL Unicode};SERVER=localhost;DATABASE=TestDB;UID=postgres;PWD=password;"
    const std::wstring connStr =
        L"DRIVER={ODBC Driver 18 for SQL Server};"
        L"SERVER=localhost;"
        L"DATABASE=TestDB;"
        L"UID=sa;"
        L"PWD=password;";

    try
    {
        DbConnection db(connStr);
        db.connect();
        std::cout << "DB connection successful\n";

        db.executeNonQuery(
            L"INSERT INTO Users (Name, Email) VALUES (N'홍길동', N'hong@example.com')"
        );
        std::cout << "Data inserted successfully\n";

        db.disconnect();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
