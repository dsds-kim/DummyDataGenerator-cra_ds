#pragma once
#include <string>

class IDbConnection
{
public:
    virtual ~IDbConnection() = default;

    virtual void      connect()                                  = 0;
    virtual void      disconnect()                               = 0;
    virtual bool      isConnected()                        const = 0;
    virtual void      executeNonQuery(const std::wstring& sql)   = 0;
    virtual long long executeScalar (const std::wstring& sql)    = 0;
};
