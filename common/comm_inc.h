#ifndef COMM_INC_H
#define COMM_INC_H

#include "macro.h"

/// system include
#ifdef __WINDOWS__
#ifndef _INC_WINDOWS
#include <WinSock2.h>
#include <ws2tcpip.h>
#endif /* _INC_WINDOWS */
#endif//__WINDOWS__

#ifdef __POSIX__
#include <unistd.h>
#include <stdint.h>       // int64_t, uint64_t
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/times.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/wait.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/epoll.h> // linux特有
#include <pthread.h>
#include <semaphore.h>
#include <typeinfo>
#include <iconv.h>
#endif//__POSIX__

#endif // COMM_INC_H
