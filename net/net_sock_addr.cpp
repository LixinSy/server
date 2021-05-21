#include "net_sock_addr.h"
#include <string.h>
#include "comm_inc.h"

SockAddr::SockAddr(uint16 family)
    : family_(family),
      sock_len_(0)
{
    memset(&data_, 0, sizeof (Data));
}

SockAddr::SockAddr(const sockaddr_in& addr)
    : family_(0),
      sock_len_(0)
{
    data_.inaddr = addr;
}

SockAddr::SockAddr(const sockaddr_in6& addr)
    : family_(0),
      sock_len_(0)
{
    data_.inaddr6 = addr;
}

SockAddr::SockAddr(const std::string& host, uint16 port)
    : family_(0),
      sock_len_(0)
{
    data_.inaddr.sin_family = AF_INET;
    data_.inaddr.sin_addr.s_addr = inet_addr(host.c_str());
    data_.inaddr.sin_port = port;
}

SockAddr::~SockAddr() {

}
