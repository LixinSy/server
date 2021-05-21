#include <bits/stdc++.h>
#include <openssl/aes.h>
#include "def.h"
#include "comm_inc.h"
#include "comm_time_util.h"
#include "core_thread.h"
#include "core_thread_pool.h"
#include "core_timer.h"
#include "core_mutex.h"
#include "msg_cd.pb.h"
#include "net_reactor.h"
#include "net_tcp_server.h"
#include "core_log.h"

using namespace std;

const char *svr_ip = "10.0.2.15";
const uint16 svr_port = 5666;

class TT: public Timer
{
public:
    TT(){}
    virtual void on_timer() {
        uint64 t = TimeUtil::get_millisecond();
        printf("timer id = %lu, now = %lu\n", get_timer_id(), t);
    }
};

int main()
{
    srand(time(nullptr));
    printf("pid = %d, __cplusplus = %ld, long=%lu\n", getpid(), __cplusplus, sizeof(void*));

    XLog::instance()->start();
    TimerManager::instance()->start();

    Reactor reactor(0);
    //reactor.register_acceptor()
    reactor.loop();

    printf("%d\n", 0);
    getchar();
    return 0;
}


void loop(){
    int listen_fd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in addrin, client_addr;
    addrin.sin_addr.s_addr = inet_addr(svr_ip);
    addrin.sin_port = svr_port;
    if (::bind(listen_fd, (struct sockaddr*)&addrin, sizeof (struct sockaddr_in))) {
        fprintf(stderr, "bind err\n");
    }
    if (::listen(listen_fd, 1024)) {
        fprintf(stderr, "listen err\n");
    }

    int epfd = epoll_create(0);
    struct epoll_event lsev;
    lsev.events = EPOLLIN;
    lsev.data.fd = listen_fd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, listen_fd, &lsev);

    byte buff[102400];
    int conn_fd, ev_num, msg_len, err;
    socklen_t sock_len;
    struct epoll_event evs[512], ev;
    while (1) {
        ev_num = epoll_wait(epfd, evs, 512, 5);
        err = (ev_num == -1) ? errno : 0;
        if (err == EINTR) {

        }
        for (int i = 0; i < ev_num; ++i) {
            struct epoll_event &cur_e = evs[i];
            if (cur_e.data.fd == listen_fd) { //client conn
                conn_fd = ::accept(listen_fd, (struct sockaddr*)&client_addr, &sock_len);
                ev.events = EPOLLIN;
                ev.data.fd = conn_fd;
                epoll_ctl(epfd, EPOLL_CTL_ADD, conn_fd, &ev);
                continue;
            }
            if (cur_e.events & EPOLLIN) {
                msg_len = ::recv(cur_e.data.fd, buff, 102400, 0);
                if (msg_len == 0) { //FIN
                    ::close(cur_e.data.fd);
                    epoll_ctl(epfd, EPOLL_CTL_DEL, cur_e.data.fd, &ev);
                } else if (msg_len > 0) { //compute
                    printf("%s\n", buff);
                    int nbyte = ::send(cur_e.data.fd, buff, msg_len, 0);
                    if (nbyte == -1) {
                        if (errno == EWOULDBLOCK) {
                            ev.events = EPOLLOUT;
                            ev.data.fd = cur_e.data.fd;
                            epoll_ctl(epfd, EPOLL_CTL_MOD, conn_fd, &ev);
                        }
                        if (errno == EINTR) {
                            nbyte = ::send(cur_e.data.fd, buff, msg_len, 0);
                        }
                        if (errno == EPIPE) {
                            close(cur_e.data.fd);
                        }
                    }

                } else { //recv err
                    //du huan chong qu wei kong /da duan
                    if (errno == EWOULDBLOCK || errno == EINTR){
                        //recv
                    }
                    if (errno == ETIMEDOUT) {
                        close(cur_e.data.fd);
                    }
                }
            }
            if (cur_e.events & EPOLLOUT) {
//                if (cur_e.data.fd == mysqlfd) {

//                } else {
//                    err = ::send(cur_e.data.fd, buff, msg_len, 0);
//                }

            }
            if (cur_e.events & EPOLLERR) {

            }
            if (cur_e.events & EPOLLHUP || cur_e.events & EPOLLRDHUP) {
                ::close(cur_e.data.fd);
                epoll_ctl(epfd, EPOLL_CTL_DEL, cur_e.data.fd, &ev);
            }
        }
    }
}
