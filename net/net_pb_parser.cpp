#include "net_pb_parser.h"


PBParser::PBParser() {
    read_buffer_ = new ByteBuffer(512);
    write_buffer_ = new ByteBuffer(1024);
}

PBParser::~PBParser() {
    delete read_buffer_;
    delete write_buffer_;
}

int PBParser::to_read_buffer(const void *src, int len) {
    if (len + read_buffer_->len < sizeof(uint32)){
        read_buffer_->append(src, len);
        return len;
    }
    byte *d = read_buffer_->mutable_data();
    int need = *(uint32*)d - read_buffer_->len;
    if (need > 0) {
        int l = (need > len) ? len : need;
        read_buffer_->append(src, l);
        return l;
    }
    else {
        return 0;
    }
}

int PBParser::need_bytes() {
    if (read_buffer_->len < sizeof(uint32)){
        return 1;
    }
    byte *d = read_buffer_->mutable_data();
    return *(uint32*)d - read_buffer_->len;
}

BaseMessage *PBParser::get_message() {
    BaseMessage *message = nullptr;
    return message;
}

int PBParser::to_write_buffer(const BaseMessage *msg) {
    write_buffer_->append(0, 0);
    return 0;
}

ByteBuffer* PBParser::get_write_buffer() {
    return write_buffer_;
}
