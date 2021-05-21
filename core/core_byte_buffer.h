#ifndef CORE_BYTE_BUFFER_H
#define CORE_BYTE_BUFFER_H

#include "def.h"

struct ByteBuffer
{
    ByteBuffer(uint32 size);
    ~ByteBuffer();
    void append(const void* src, uint32 byte_len);
    void reset() { len = 0; }
    byte *mutable_data() { return data; }
    const byte *get_data() const { return data; };
    uint32 get_length() const { return len; }

    byte *data;
    uint32 size;
    uint32 len;
};

#endif // CORE_BYTE_BUFFER_H
