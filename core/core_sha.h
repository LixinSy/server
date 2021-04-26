#ifndef CORE_SHA_H
#define CORE_SHA_H

#include <string>
#include "def.h"


class SHA256
{
    enum { BlockSize = 64, HashBytes = 32 };
    enum { HashValues = HashBytes / 4 };
public:
    SHA256();
    std::string operator()(const void* data, size_t num_bytes);
    std::string operator()(const std::string& text);
    void add(const void* data, size_t num_bytes);
    std::string get_hash();
    void get_hash(uint8 buffer[HashBytes]);
    void reset();

private:
    void _process_block(const void* data);
    void _process_buffer();

    uint64      num_bytes_;
    size_t      buffer_size_;
    uint8       buffer_[BlockSize];
    uint32      hash_[HashValues];
};


#endif // CORE_SHA_H
