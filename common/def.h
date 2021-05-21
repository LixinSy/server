#ifndef DEF_H
#define DEF_H

#include "macro.h"

/// typedef
#ifdef __WINDOWS__
typedef unsigned __int8      byte;
typedef __int8               int8;
typedef unsigned __int8      uint8;
typedef __int16              int16;
typedef unsigned __int16     uint16;
typedef __int32              int32;
typedef unsigned __int32     uint32;
typedef __int64              int64;
typedef unsigned __int64     uint64;
typedef float                float32;
typedef double               float64;
typedef int                  socklen_t;
typedef unsigned long        pthread_t;
#define I64D "%I64d"
#define I64U "%I64u"
#endif//__WINDOWS__

#ifdef __POSIX__
#include <stdint.h>
typedef uint8_t              byte;
typedef int8_t               int8;
typedef uint8_t              uint8;
typedef int16_t              int16;
typedef uint16_t             uint16;
typedef int32_t              int32;
typedef uint32_t             uint32;
typedef int64_t              int64;
typedef uint64_t             uint64;
typedef float                float32;
typedef double               float64;
typedef unsigned short       WORD;
typedef unsigned long        DWORD;
typedef int                  SOCKET;
typedef int                  BOOL;
typedef int                  HANDLE;
#define TRUE                 (1)
#define FALSE                (0)
#define INVALID_SOCKET       (SOCKET)(~0)
#define INVALID_HANDLE_VALUE (HANDLE)(-1)
#define SOCKET_ERROR         (-1)
#define __stdcall
#define I64D "%ld"
#define I64U "%lu"
#endif//__POSIX__

/// define
#define LX_OK          0
#define LX_ERROR      -1
#define LX_AGAIN      -2
#define LX_BUSY       -3
#define LX_DONE       -4

#ifndef offsetof
#ifdef __compiler_offsetof
#define offsetof(TYPE,MEMBER) __compiler_offsetof(TYPE,MEMBER)
#else
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif
#endif



#endif // DEF_H
