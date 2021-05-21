#ifndef NET_CONNECTION_H
#define NET_CONNECTION_H

#include <assert.h>
#include "comm_inc.h"
#include "net_socket.h"
#include "net_sock_addr.h"
#include "net_parser.h"
#include "net_event_module.h"

class Connection;


class Connection
{
    enum State
    {
        kDisconnected,
        kConnecting,
        kConnected,
        kDisconnecting
    };
public:
    Connection();
    Connection(Socket *sock, SockAddr *addr, Parser *parser);
    ~Connection();
    static void business_thread(void *arg) {
        Connection *conn = (Connection *)arg;
    }
    void attach(EventModule *m) {
        event_module_ = m;
    }
    bool read_handle(NetEvent *net_event) {
        assert(net_event->fd == sock_->fd());
        byte buff[10240];
        int msg_len = ::recv(sock_->fd(), buff, 10240, 0);
        if (msg_len == 0) { //FIN
            event_module_->remove_event(net_event);
        } else if (msg_len > 0) { //compute

            printf("recv: %s\n", buff);
            int nbyte = ::send(sock_->fd(), buff, msg_len, 0);
            if (nbyte == -1) {
                if (errno == EWOULDBLOCK) {
                    net_event->enable_write();
                    event_module_->update_event(net_event);
                } else if (errno == EINTR) {
                    nbyte = ::send(sock_->fd(), buff, msg_len, 0);
                } else if (errno == EPIPE) {
                    event_module_->remove_event(net_event);
                }
            }
        } else { //recv err
            if (errno == EWOULDBLOCK || errno == EINTR){
                msg_len = ::recv(sock_->fd(), buff, 10240, 0);
            } else if (errno == ETIMEDOUT) {
                event_module_->remove_event(net_event);
            }
        }
        return true;
    }
    bool write_handle(NetEvent *net_event);
    void close_handle(NetEvent *net_event) {

    }
private:
    Socket *sock_;
    SockAddr *addr_;
    Parser *parser_;
    EventModule *event_module_;
    bool in_task_queue_;
};

#endif // NET_CONNECTION_H
