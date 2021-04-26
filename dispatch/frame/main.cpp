#include <bits/stdc++.h>
#include "def.h"
#include "comm_inc.h"
#include "comm_time_util.h"
#include "core_thread.h"
#include "core_thread_pool.h"
#include "core_timer.h"
#include "core_mutex.h"

using namespace std;


class TT: public Timer
{
public:
    TT(){}
    virtual void on_timer() {
        uint64 t = TimeUtil::get_millisecond();
        printf("timer id = %lu, now = %lu\n", get_timer_id(), t);
    }
};

void nett() {

}


int main()
{
    srand(time(nullptr));
    printf("pid = %lu, __cplusplus = %ld\n", pthread_self(), __cplusplus);

//    int listen_fd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//    struct sockaddr_in addrin;
//    addrin.sin_addr.s_addr = htonl(INADDR_ANY);
//    addrin.sin_port = 18000;
//    if (::bind(listen_fd, (struct sockaddr*)&addrin, sizeof (struct sockaddr_in))) {
//        fprintf(stderr, "bind err\n");
//    }
//    if (::listen(listen_fd, 1024)) {
//        fprintf(stderr, "listen err\n");
//    }

//    int epfd = epoll_create(0);
//    struct epoll_event lsev;
//    lsev.events = EPOLLIN;
//    lsev.data.fd = listen_fd;
//    epoll_ctl(epfd, EPOLL_CTL_ADD, listen_fd, &lsev);

//    byte buff[102400];
//    int conn_fd, ev_num, msg_len, err;
//    socklen_t sock_len;
//    struct epoll_event evs[512], ev;
//    while (1) {
//        ev_num = epoll_wait(epfd, evs, 512, 5);
//        err = (ev_num == -1) ? errno : 0;
//        if (err == EINTR) {

//        }
//        for (int i = 0; i < ev_num; ++i) {
//            if (evs[i].data.fd == listen_fd) { //client conn
//                conn_fd = ::accept(listen_fd, (struct sockaddr*)&addrin, &sock_len);
//                ev.events |= EPOLLIN;
//                ev.data.fd = conn_fd;
//                epoll_ctl(epfd, EPOLL_CTL_ADD, conn_fd, &ev);
//                continue;
//            }
//            if (evs[i].events & EPOLLIN) {
//                msg_len = ::recv(evs[i].data.fd, buff, 102400, 0);
//                if (msg_len == 0) { //FIN
//                    ::close(evs[i].data.fd);
//                    epoll_ctl(epfd, EPOLL_CTL_DEL, evs[i].data.fd, &ev);
//                } else if (msg_len > 0) { //compute
//                    printf("%s\n", buff);
//                    err = ::send(evs[i].data.fd, buff, msg_len, 0);
//                    ev.events |= EPOLLOUT;
//                    ev.data.fd = evs[i].data.fd;
//                    epoll_ctl(epfd, EPOLL_CTL_MOD, conn_fd, &ev);
//                } else { //recv err
//                }
//            }
//            if (evs[i].events & EPOLLOUT) {

//            }
//            if (evs[i].events & EPOLLERR) {

//            }
//            if (evs[i].events & EPOLLHUP || evs[i].events & EPOLLRDHUP) {
//                ::close(evs[i].data.fd);
//                epoll_ctl(epfd, EPOLL_CTL_DEL, evs[i].data.fd, &ev);
//            }
//        }
//        TimerManager::instance()->expire_timer();
//    }

    printf("%d\n", 0);
    return 0;
}
