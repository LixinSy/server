#include "net_sock_channel.h"
#include "net_event_module.h"
#include "net_session.h"


static void read_thread_func(void *e) {
    SockChannel *chan = static_cast<SockChannel*>(e);
    chan->read();
    chan->release();
}


SockChannel::SockChannel(EventModule *m, Socket *sock, Parser *parser)
    : NetEvent(m),
      sock_(sock),
      parser_(parser),
      recving_mutex_(false),
      sending_mutex_(false)
{
}

SockChannel::~SockChannel() {
    sending_mutex_.unlock();
    recving_mutex_.unlock();
    if (parser_) {
        delete parser_;
    }
    if (sock_) {
        delete sock_;
    }
}

void SockChannel::read(){
    byte buff[10240];
    while (true) {
        int recv_len = ::recv(sock_->fd(), buff, 10240, 0);
        if (recv_len > 0) {
            int put_len = 0;
            for (int len = 0; put_len < recv_len; put_len += len) {
                len = parser_->to_read_buffer(buff + put_len, recv_len - put_len);
                if (parser_->need_bytes() > 0) //没有接收完整，继续接收
                    break;
                BaseMessage *msg = parser_->get_message();
                parser_->reset_read_buffer();
            }
        } else if (recv_len == 0) {
            handle_close();
            return;
        } else {
            if (errno == EWOULDBLOCK || errno == EAGAIN) {
                return;
            } else if (errno == EINTR) {
                continue;
            } else {
                handle_close();
                return;
            }
        }
    }
}

void SockChannel::write(BaseMessage* msg) {
    parser_->to_write_buffer(msg);
    ByteBuffer *buffer = parser_->get_write_buffer();
    const byte *src = buffer->get_data();
    int byte_len = buffer->get_length();
    if (byte_len <= 0) {
        return;
    }
    while (true) {
        int err = ::send(sock_->fd(), src, byte_len, 0);
        if (err == byte_len) {
            buffer->reset();
            return;
        } else {
            if (errno == EWOULDBLOCK || errno == EAGAIN) {
                enable_output();
                event_module_->update_event(this);
                return;
            } else if (errno == EINTR) {
                continue;
            } else {
                handle_close();
                return;
            }
        }
    }
}

int SockChannel::handle_input() {
    add_ref();
    if (!event_module_->push_task(read_thread_func, this)) {
        read();
        release();
    }
    return LX_OK;
}

int SockChannel::handle_output() {
    ByteBuffer *buffer = parser_->get_write_buffer();
    const byte *src = buffer->get_data();
    int byte_len = buffer->get_length();
    if (byte_len <= 0) {
        disable_output();
        event_module_->update_event(this);
        return LX_OK;
    }
    while (true) {
        int err = ::send(sock_->fd(), src, byte_len, 0);
        if (err == byte_len) {
            buffer->reset();
            disable_output();
            event_module_->update_event(this);
            return LX_OK;
        } else {
            if (errno == EWOULDBLOCK || errno == EAGAIN) {
                //enable_output();
                //event_module_->update_event(this);
                return LX_OK;
            } else if (errno == EINTR) {
                continue;
            } else {
                handle_close();
                return LX_ERROR;
            }
        }
    }
    return LX_OK;
}

int SockChannel::handle_close() {
    SessionPtr session = tied_session_.lock();
    if (session) {
        session->close_session();
        tied_session_.reset();
    }
    disable_all();
    event_module_->update_event(this);
    return 0;
}

void SockChannel::attach_session(SessionPtr session_ptr) {
    tied_session_ = session_ptr;
}
