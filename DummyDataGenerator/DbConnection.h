#pragma once
#include "IDbConnection.h"
#include <windows.h>
#include <sqlext.h>
#include <stdexcept>

class DbConnection : public IDbConnection
{
public:
    explicit DbConnection(const std::wstring& connectionString);
    ~DbConnection();

    DbConnection(const DbConnection&) = delete;
    DbConnection& operator=(const DbConnection&) = delete;

    void connect();
    void disconnect();
    bool isConnected() const;

    // DML/DDL 실행 (INSERT, UPDATE, DELETE, CREATE 등)
    void executeNonQuery(const std::wstring& sql);

    // 단일 정수값을 반환하는 SELECT 실행 (COUNT 등)
    long long executeScalar(const std::wstring& sql);

    // 연결 핸들을 직접 사용해야 할 경우 노출
    SQLHDBC handle() const { return m_hdbc; }

private:
    static std::string buildErrorMessage(SQLSMALLINT handleType, SQLHANDLE handle, const char* context);

    std::wstring m_connectionString;
    SQLHENV      m_henv = SQL_NULL_HENV;
    SQLHDBC      m_hdbc = SQL_NULL_HDBC;
    bool         m_connected = false;
};
