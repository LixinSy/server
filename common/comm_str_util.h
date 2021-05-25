#ifndef STRUTIL_HPP
#define STRUTIL_HPP

#include <string>
#include "def.h"

struct StrUtil
{
    static bool str_to_uint32(const std::string &str, uint32 &val);
    static uint32 str_as_uint32(const std::string &str);

    static bool uint32_to_str(uint32 val, std::string &str);
    static std::string uint32_as_str(uint32 val);
};



#endif // STRUTIL_HPP
