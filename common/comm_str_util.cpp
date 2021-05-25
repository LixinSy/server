#include "comm_str_util.h"
#include <sstream>


bool StrUtil::str_to_uint32(const std::string &str, uint32 &val) {
    return true;
}

uint32 StrUtil::str_as_uint32(const std::string &str) {
    return 0;
}

bool StrUtil::uint32_to_str(uint32 val, std::string &str) {
    str = std::to_string(val);
    return true;
}

std::string StrUtil::uint32_as_str(uint32 val) {
    return std::to_string(val);
}
