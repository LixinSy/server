#ifndef CORE_MURMURHASH_H
#define CORE_MURMURHASH_H

#include "def.h"

#ifdef __cplusplus
extern "C" {
#endif


uint32 murmur_hash2 (const void * key, int len, uint32 seed);

void murmur_hash3_x86_32  (const void * key, int len, uint32 seed, void * out);

void murmur_hash3_x86_128 (const void * key, int len, uint32 seed, void * out);

void murmur_hash3_x64_128 (const void * key, int len, uint32 seed, void * out);


#ifdef __cplusplus
}
#endif


#endif // CORE_MURMURHASH_H
