#pragma once
#include "IDbConnection.h"
#include <stdexcept>
#include <vector>

class MockDbConnection : public IDbConnection
{
public:
    // --- Test configuration ---
    void setThrowOnConnect (bool v) { m_throwOnConnect  = v; }
    void setThrowOnExecute (bool v) { m_throwOnExecute  = v; }
    void setScalarResult   (long long v) { m_scalarResult = v; }

    // --- Test inspection ---
    const std::vector<std::wstring>& executedStatements() const { return m_statements; }

    // --- IDbConnection ---
    void connect() override
    {
        if (m_connected) return; // idempotent, same as real DbConnection
        if (m_throwOnConnect)
            throw std::runtime_error("MockDbConnection: connect failed");
        m_connected = true;
    }

    void disconnect() override { m_connected = false; }

    bool isConnected() const override { return m_connected; }

    void executeNonQuery(const std::wstring& sql) override
    {
        if (!m_connected)
            throw std::runtime_error("not connected to database");
        if (m_throwOnExecute)
            throw std::runtime_error("MockDbConnection: execute failed");
        m_statements.push_back(sql);
    }

    long long executeScalar(const std::wstring& sql) override
    {
        if (!m_connected)
            throw std::runtime_error("not connected to database");
        m_statements.push_back(sql);
        return m_scalarResult;
    }

private:
    bool      m_connected      = false;
    bool      m_throwOnConnect = false;
    bool      m_throwOnExecute = false;
    long long m_scalarResult   = 0;
    std::vector<std::wstring> m_statements;
};
