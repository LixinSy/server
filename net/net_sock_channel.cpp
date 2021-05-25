#include "net_sock_channel.h"
#include "net_event_module.h"
#include "net_pb_parser.h"
#include "core_byte_buffer.h"
#include "net_socket.h"
#include "net_session.h"


#define RECV_MSG_BUF_SIZE 1024


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
    fd_ = sock_->fd();
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
    LockGuard<Mutex> lock(this->recving_mutex_);
    int recv_len;
    byte buff[RECV_MSG_BUF_SIZE];
    while (true) {
        recv_len = ::recv(sock_->fd(), buff, RECV_MSG_BUF_SIZE, 0);
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
            int err = errno;
            if (err == EWOULDBLOCK || err == EAGAIN) {
                return;
            } else if (err == EINTR) {
                continue;
            } else {
                handle_close();
                return;
            }
        }
    }
}

void SockChannel::write(BaseMessage* msg) {
    LockGuard<Mutex> lock(this->sending_mutex_);
    parser_->to_write_buffer(msg);
    ByteBuffer *buffer = parser_->get_write_buffer();
    const byte *src = buffer->get_data();
    int byte_len = buffer->get_length();
    if (byte_len <= 0) {
        return;
    }
    int len;
    while (true) {
        len = ::send(sock_->fd(), src, byte_len, 0);
        if (len == byte_len) {
            buffer->reset();
            return;
        } else {
            int err = errno;
            if (err == EWOULDBLOCK || err == EAGAIN) {
                enable_output();
                event_module_->update_event(this);
                return;
            } else if (err == EINTR) {
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
    LockGuard<Mutex> lock(this->sending_mutex_);
    ByteBuffer *buffer = parser_->get_write_buffer();
    const byte *src = buffer->get_data();
    int byte_len = buffer->get_length();
    if (byte_len <= 0) {
        disable_output();
        event_module_->update_event(this);
        return LX_OK;
    }
    int len;
    while (true) {
        len = ::send(sock_->fd(), src, byte_len, 0);
        if (len == byte_len) {
            buffer->reset();
            disable_output();
            event_module_->update_event(this);
            return LX_OK;
        } else {
            int err = errno;
            if (err == EWOULDBLOCK || err == EAGAIN) {
                //enable_output();
                //event_module_->update_event(this);
                return LX_OK;
            } else if (err == EINTR) {
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
    return LX_OK;
}

void SockChannel::attach_session(SessionPtr session_ptr) {
    tied_session_ = session_ptr;
}
