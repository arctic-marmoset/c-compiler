#ifndef C_COMPILER_TOKEN_H
#define C_COMPILER_TOKEN_H

#include "token_type.h"

#include <cstddef>
#include <sstream>
#include <string>

namespace cc {

struct source_position
{
    std::size_t line;
    std::size_t column;

    std::string to_string(const std::string &open = "(",
                          const std::string &close = ")") const
    {
        std::ostringstream ss;
        ss << open << line << "," << column << close;
        return ss.str();
    }
};

struct token
{
    const cc::token_type type;
    const std::string text;
    const source_position pos;
};

}

inline std::ostream &operator<<(std::ostream &os, cc::source_position pos)
{
    return os << pos.to_string();
}

#endif
