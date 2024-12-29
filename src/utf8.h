#pragma once
#include <string>

#include "exception.h"

namespace utf8 {
    char32_t utf8to32(const std::string & str, size_t & strOffset);
    std::string utf32to8(char32_t codepoint);
    constexpr char32_t DECODING_ERROR = 0xffffffffUi32;
}
