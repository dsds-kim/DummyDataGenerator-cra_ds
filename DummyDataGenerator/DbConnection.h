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

    void      executeNonQuery(const std::wstring& sql);
    long long executeScalar  (const std::wstring& sql); // single-value SELECT (e.g. COUNT)

    SQLHDBC handle() const { return m_hdbc; }

private:
    static std::string buildErrorMessage(SQLSMALLINT handleType, SQLHANDLE handle, const char* context);

    std::wstring m_connectionString;
    SQLHENV      m_henv = SQL_NULL_HENV;
    SQLHDBC      m_hdbc = SQL_NULL_HDBC;
    bool         m_connected = false;
};
