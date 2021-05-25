#include "net_session.h"
#include "net_sock_channel.h"

Session::Session() {
    channel_ = nullptr;
}

Session::~Session() {

}

bool Session::open_session(SockChannel* chan) {
    channel_ = chan;
    channel_->attach_session(shared_from_this());
    on_opened_session();
    return true;
}

bool Session::close_session() {
    channel_ = nullptr;
    on_closed_session();
    return true;
}

bool Session::is_alive() {
    return channel_ != nullptr;
}
