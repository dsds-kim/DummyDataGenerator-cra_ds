#include "DbConnection.h"
#include <format>

DbConnection::DbConnection(const std::wstring& connectionString)
    : m_connectionString(connectionString)
{}

DbConnection::~DbConnection()
{
    disconnect();
}

void DbConnection::connect()
{
    if (m_connected) return;

    if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_henv) != SQL_SUCCESS)
        throw std::runtime_error("ODBC: failed to allocate environment handle");

    SQLSetEnvAttr(m_henv, SQL_ATTR_ODBC_VERSION, reinterpret_cast<SQLPOINTER>(SQL_OV_ODBC3), 0);

    if (SQLAllocHandle(SQL_HANDLE_DBC, m_henv, &m_hdbc) != SQL_SUCCESS)
        throw std::runtime_error("ODBC: failed to allocate connection handle");

    SQLWCHAR outConnStr[1024]{};
    SQLSMALLINT outLen = 0;

    SQLRETURN rc = SQLDriverConnectW(
        m_hdbc,
        nullptr,
        const_cast<SQLWCHAR*>(m_connectionString.c_str()),
        SQL_NTS,
        outConnStr, static_cast<SQLSMALLINT>(std::size(outConnStr)),
        &outLen,
        SQL_DRIVER_NOPROMPT
    );

    if (rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO)
        throw std::runtime_error(buildErrorMessage(SQL_HANDLE_DBC, m_hdbc, "SQLDriverConnect"));

    m_connected = true;
}

void DbConnection::disconnect()
{
    if (m_hdbc != SQL_NULL_HDBC)
    {
        if (m_connected)
        {
            SQLDisconnect(m_hdbc);
            m_connected = false;
        }
        SQLFreeHandle(SQL_HANDLE_DBC, m_hdbc);
        m_hdbc = SQL_NULL_HDBC;
    }

    if (m_henv != SQL_NULL_HENV)
    {
        SQLFreeHandle(SQL_HANDLE_ENV, m_henv);
        m_henv = SQL_NULL_HENV;
    }
}

bool DbConnection::isConnected() const
{
    return m_connected;
}

void DbConnection::executeNonQuery(const std::wstring& sql)
{
    if (!m_connected)
        throw std::runtime_error("not connected to database");

    SQLHSTMT hstmt = SQL_NULL_HSTMT;
    if (SQLAllocHandle(SQL_HANDLE_STMT, m_hdbc, &hstmt) != SQL_SUCCESS)
        throw std::runtime_error("ODBC: failed to allocate statement handle");

    SQLRETURN rc = SQLExecDirectW(hstmt, const_cast<SQLWCHAR*>(sql.c_str()), SQL_NTS);

    if (rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO)
    {
        std::string msg = buildErrorMessage(SQL_HANDLE_STMT, hstmt, "SQLExecDirect");
        SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
        throw std::runtime_error(msg);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
}

long long DbConnection::executeScalar(const std::wstring& sql)
{
    if (!m_connected)
        throw std::runtime_error("not connected to database");

    SQLHSTMT hstmt = SQL_NULL_HSTMT;
    if (SQLAllocHandle(SQL_HANDLE_STMT, m_hdbc, &hstmt) != SQL_SUCCESS)
        throw std::runtime_error("ODBC: failed to allocate statement handle");

    SQLRETURN rc = SQLExecDirectW(hstmt, const_cast<SQLWCHAR*>(sql.c_str()), SQL_NTS);
    if (rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO)
    {
        std::string msg = buildErrorMessage(SQL_HANDLE_STMT, hstmt, "SQLExecDirect");
        SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
        throw std::runtime_error(msg);
    }

    if (SQLFetch(hstmt) != SQL_SUCCESS)
    {
        SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
        throw std::runtime_error("executeScalar: no rows returned");
    }

    SQLBIGINT value = 0;
    SQLLEN indicator = 0;
    SQLGetData(hstmt, 1, SQL_C_SBIGINT, &value, sizeof(value), &indicator);
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);

    return static_cast<long long>(value);
}

std::string DbConnection::buildErrorMessage(SQLSMALLINT handleType, SQLHANDLE handle, const char* context)
{
    SQLWCHAR sqlState[6]{};
    SQLWCHAR message[SQL_MAX_MESSAGE_LENGTH]{};
    SQLINTEGER nativeError = 0;
    SQLSMALLINT msgLen = 0;

    SQLGetDiagRecW(handleType, handle, 1, sqlState, &nativeError, message, SQL_MAX_MESSAGE_LENGTH, &msgLen);

    std::string msg;
    if (message[0] != L'\0')
    {
        int len = WideCharToMultiByte(CP_ACP, 0, message, -1, nullptr, 0, nullptr, nullptr);
        if (len > 0)
        {
            msg.resize(len - 1);
            WideCharToMultiByte(CP_ACP, 0, message, -1, &msg[0], len, nullptr, nullptr);
        }
    }

    return std::format("{}: [{}] {}", context, nativeError, msg);
}
