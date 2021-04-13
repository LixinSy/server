#ifndef MACRO_H
#define MACRO_H

#if !defined(__WINDOWS__) && !defined(__POSIX__)
#if defined(_MSC_VER) || defined(WIN32) || defined(_WIN32)
#define __WINDOWS__   1
#elif defined(__linux__)
#define __POSIX__  1
#elif defined(__APPLE__)
#define __POSIX__  1
#else
#error "Unrecognized OS platform"
#endif
#endif//!defined(__WINDOWS__) && !defined(__POSIX__)

#endif // MACRO_H
