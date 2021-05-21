#ifndef NET_SOCK_ADDR_H
#define NET_SOCK_ADDR_H

#include <string>
#include "def.h"
#include "comm_inc.h"

class Socket;

class SockAddr
{
    friend Socket;
    union Data
    {
        sockaddr_in  inaddr;
        sockaddr_in6  inaddr6;
    };
public:
    SockAddr(uint16 family);
    SockAddr(const sockaddr_in& addr);
    SockAddr(const sockaddr_in6& addr);
    SockAddr(const std::string& host, uint16 port); // host: ip或域名
    ~SockAddr();
    SockAddr& operator = (const SockAddr& addr);
    SockAddr& operator = (const sockaddr_in& addr);
    SockAddr& operator = (const sockaddr_in6& addr);
    std::string to_str() const;
    static bool resolve_dns(const std::string& domain, SockAddr &addr);
private:
    uint16 family_;
    uint32 sock_len_;
    Data data_;
};

#endif // NET_SOCK_ADDR_H
