#include "net_sock_addr.h"
#include <string.h>
#include "comm_inc.h"

SockAddr::SockAddr(uint16 family)
{
    memset(&data_, 0, sizeof (Data));
    memcpy(&data_, &family, sizeof (uint16));
}

SockAddr::SockAddr(const sockaddr_in& addr) {
    data_.inaddr = addr;
}

SockAddr::SockAddr(const sockaddr_in6& addr) {
    data_.inaddr6 = addr;
}

SockAddr::SockAddr(uint16 family, const std::string& host, uint16 port) {
    if (family == AF_INET) {
        data_.inaddr.sin_family = family;
        ::inet_pton(family, host.c_str(), &data_.inaddr.sin_addr);
        data_.inaddr.sin_port = port;
    }else {
        data_.inaddr6.sin6_family = family;
        ::inet_pton(family, host.c_str(), &data_.inaddr6.sin6_addr);
        data_.inaddr6.sin6_port = port;
    }
}

SockAddr::~SockAddr() {

}

std::string SockAddr::to_str() const {
    std::string str;
    uint16 family = *(uint16*)&data_;
    if (family == AF_INET) {
        str.resize(INET_ADDRSTRLEN);
        const char *ptr = ::inet_ntop(family, &data_.inaddr.sin_addr, &str[0], INET_ADDRSTRLEN);
        if (ptr == &str[0]) {
            str.append(":");
            str.append(std::to_string(data_.inaddr.sin_port));
        }
    } else {
        str.resize(INET6_ADDRSTRLEN);
        const char *ptr = ::inet_ntop(family, &data_.inaddr.sin_addr, &str[0], INET6_ADDRSTRLEN);
        if (ptr == &str[0]) {
            str.append(":");
            str.append(std::to_string(data_.inaddr6.sin6_port));
        }
    }
    return str;
}
