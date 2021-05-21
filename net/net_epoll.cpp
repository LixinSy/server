#include "net_epoll.h"

Epoll::Epoll(uint32 max_event_size)
    : EventModule(max_event_size)
{
    epfd_ = ::epoll_create(max_event_size);
}

void Epoll::wait_event(NetEventList *event_list) {
    int nready = ::epoll_wait(epfd_, events_, 128, 5);
    for (int i = 0; i < nready; ++i) {
        EventSlot *s = (EventSlot*)events_[i].data.ptr;
        NetEvent *e = s->event;
        e->active_events_ = events_[i].events;
        event_list->push_back(e);
    }
}

void Epoll::update_event(NetEvent *net_event) {
    if (net_event->state_ == NetEvent::State::NEW) {
        if (net_event->regist_events_) {
            if (add_event(net_event)) {
                net_event->state_ = NetEvent::State::ADDED;
                _operate(EPOLL_CTL_ADD, net_event);
            }
        }
    }
    else if (net_event->state_ == NetEvent::State::ADDED) {
        if (net_event->regist_events_ == 0) {
            if (del_event(net_event)) {
                net_event->state_ = NetEvent::State::DELETEED;
                _operate(EPOLL_CTL_DEL, net_event);
                net_event->release();
            }
        }
        else {
            _operate(EPOLL_CTL_MOD, net_event);
        }
    }
}

void Epoll::process_events() {
        int err;
        while (1) {
            int nready = ::epoll_wait(epfd_, events_, max_event_size_, 5);
            err = (nready == -1) ? errno : 0;
            if (err == EINTR) {
                continue;
            }
            for (int i = 0; i < nready; ++i) {
                struct epoll_event &cur_e = events_[i];
                EventSlot *s = (EventSlot*)cur_e.data.ptr;
                NetEvent *e = s->event;
                e->add_ref();
                e->active_events_ = cur_e.events;
                if (e->active_events_ & EPOLLERR || e->active_events_ & EPOLLHUP) {
                    e->handle_close();
                }
                if (e->active_events_ & EPOLLIN) {
                    e->handle_input();
                }
                if (e->active_events_ & EPOLLOUT) {
                    e->handle_output();
                }
                e->release();
            }
        }
    }

void Epoll::_operate(int opt, NetEvent *net_event) {
    struct epoll_event event;
    event.events = net_event->regist_events_;
    event.data.ptr = net_event->slot_;
    if (::epoll_ctl(epfd_, opt, net_event->fd_, &event) < 0) {

    }
}
