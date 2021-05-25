#include "core_byte_buffer.h"
#include <string.h>

#define INCREMENT_SIZE 32

ByteBuffer::ByteBuffer(uint32 size) {
    this->len = 0;
    this->size = size;
    this->data = new byte[size];
    memset(data, 0, size);
}

ByteBuffer::~ByteBuffer() {
    delete data;
}

void ByteBuffer::append(const void* src, uint32 byte_len){
    if (byte_len + len > size) {
        size = (byte_len + len);
        byte *new_data = new byte[size];
        memcpy(new_data, data, len);
        delete data;
        data = new_data;
        memcpy(data+len, src, byte_len);
        len += byte_len;
    }
    else{
        memcpy(data+len, src, byte_len);
        len += byte_len;
    }
}
