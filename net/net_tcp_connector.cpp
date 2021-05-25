#include "net_tcp_connector.h"
#include <assert.h>
#include "net_event_module.h"
#include "net_socket.h"
#include "net_pb_parser.h"
#include "net_sock_channel.h"


TcpConnector::TcpConnector(EventModule *m, Socket *sock, ParserCreator *creator)
    : NetEvent(m),
      state_(IDLE),
      sock_(sock),
      parser_creator_(creator)
{
    assert(sock);
    assert(creator);
    fd_ = sock_->fd();
}

TcpConnector::~TcpConnector() {
    if (parser_creator_) delete parser_creator_;
}

int TcpConnector::handle_input() {
    return LX_OK;
}

int TcpConnector::handle_output() {
    if (state_ == State::CONNECTING) {
        state_ = State::CONNECTED;
    }
    return LX_OK;
}

int TcpConnector::handle_close() {
    IConnectorPtr callback = conn_call_back_.lock();
    if (callback) {
        callback->on_failed();
        callback.reset();
    }
    disable_all();
    event_module_->update_event(this);
    return LX_OK;
}

SockChannel *TcpConnector::get_channel() {
    if (!sock_->is_nonblock() && !sock_->set_nonblock()) {
        return nullptr;
    }
    SockChannel *chan = new SockChannel(event_module_, sock_, parser_creator_->create());
    chan->enable_input();
    event_module_->update_event(chan);
    return chan;
}

void TcpConnector::regist_callback(IConnectorPtr callback) {
    assert(callback.get());
    conn_call_back_ = callback;
}

