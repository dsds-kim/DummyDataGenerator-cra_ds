#include "DbConnectionTest.h"
#include "MockDbConnection.h"
#include "TestRunner.h"

void registerDbConnectionTests(TestRunner& runner)
{
    runner.add("connect_invalid_string_throws", [] {
        MockDbConnection db;
        db.setThrowOnConnect(true);
        ASSERT_THROWS(db.connect());
    });

    runner.add("execute_without_connect_throws", [] {
        MockDbConnection db;
        ASSERT_THROWS(db.executeNonQuery(L"SELECT 1"));
    });

    runner.add("connect_success", [] {
        MockDbConnection db;
        db.connect();
        ASSERT_TRUE(db.isConnected());
    });

    runner.add("disconnect_resets_state", [] {
        MockDbConnection db;
        db.connect();
        db.disconnect();
        ASSERT_TRUE(!db.isConnected());
    });

    runner.add("double_connect_is_idempotent", [] {
        MockDbConnection db;
        db.connect();
        db.connect(); // second call must be a no-op, not throw
        ASSERT_TRUE(db.isConnected());
    });

    runner.add("execute_nonquery_insert", [] {
        MockDbConnection db;
        db.connect();
        db.setScalarResult(1);

        db.executeNonQuery(L"CREATE TABLE ##TestInsert (Id INT)");
        db.executeNonQuery(L"INSERT INTO ##TestInsert (Id) VALUES (1)");
        long long count = db.executeScalar(L"SELECT COUNT(*) FROM ##TestInsert");
        db.executeNonQuery(L"DROP TABLE ##TestInsert");

        ASSERT_TRUE(count == 1);
        ASSERT_TRUE(db.executedStatements().size() == 4);
    });

    runner.add("execute_invalid_sql_throws", [] {
        MockDbConnection db;
        db.connect();
        db.setThrowOnExecute(true);
        ASSERT_THROWS(db.executeNonQuery(L"THIS IS NOT VALID SQL"));
    });
}
