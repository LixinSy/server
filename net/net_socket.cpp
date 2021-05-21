#include "net_socket.h"

Socket::Socket()
    : fd_(0),
      closed_(false),
      nonblock_(false)
{
}

Socket::Socket(int fd)
    : fd_(fd),
      closed_(false),
      nonblock_(false)
{
}

Socket::~Socket() {
    if (!closed_) {
        ::close(fd_);
    }
}

int Socket::fd() const {
    return fd_;
}

void Socket::close() {
    if (!closed_) {
        ::close(fd_);
        closed_ = true;
    }
}

bool Socket::closed() const {
    return closed_;
}

int Socket::set_nonblock() {
    int old_option = fcntl(fd_, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd_, F_SETFL, new_option);
    return old_option;
}

bool Socket::is_nonblock() const {
    return nonblock_;
}

bool Socket::set_sock_opt(int opt, void *val, socklen_t vallen) {
    if (::setsockopt(fd_, SOL_SOCKET, opt, val, vallen) == 0) {
        return true;
    }
    return false;
}

bool Socket::get_sock_opt(int opt, void *val, socklen_t *vallen) {
    if (::getsockopt(fd_, SOL_SOCKET, opt, val, vallen) == 0) {
        return true;
    }
    return false;
}

Socket * Socket::open(int family, int type, int protocol) {
    int fd = ::socket(family, type, protocol);
    if (fd > 0) {
        return new Socket(fd);
    } else {
        return nullptr;
    }
}

Socket * Socket::accept(int server_fd, SockAddr &sock_addr) {
    int conn_fd = ::accept(server_fd, (struct sockaddr*)&sock_addr.data_, &sock_addr.sock_len_);
    if (conn_fd > 0) {
        return new Socket(conn_fd);
    } else {
        return nullptr;
    }
}
