#ifndef NET_SOCKET_H
#define NET_SOCKET_H

#include <string>
#include <unordered_map>
#include "def.h"
#include "net_sock_addr.h"


class Socket
{
public:
    Socket();
    Socket(int fd);
    ~Socket();
    int fd() const { return fd_; }
    void close();
    bool closed() const;
    bool set_nonblock();
    bool is_nonblock() const;
    bool set_sock_opt(int opt, void *val, socklen_t vallen);
    bool get_sock_opt(int opt, void *val, socklen_t *vallen);
    static Socket * open(int family, int type, int protocol);
    static Socket * accept(int server_fd, SockAddr &sock_addr);
private:
    int fd_;
    bool closed_;
    bool nonblock_;
    std::unordered_map<int, void*> map_opt_; //<opt, val>
};


#endif // NET_SOCKET_H
