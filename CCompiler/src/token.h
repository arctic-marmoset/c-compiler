#ifndef C_COMPILER_TOKEN_H
#define C_COMPILER_TOKEN_H

#include "token_type.h"
#include <cstddef>
#include <sstream>
#include <string>

struct source_position
{
    std::size_t line;
    std::size_t column;

    std::string to_string(std::string open = "(", std::string close = ")") const
    {
        std::ostringstream ss;
        ss << open << line << "," << column << close;
        return ss.str();
    }
};

struct token
{
    const token_type type;
    const std::string text;
    const source_position pos;

    token(token_type type, std::size_t line, std::size_t column, std::string text)
        : type(type)
        , text(std::move(text))
        , pos({ line, column })
    {
    }
};

inline std::ostream &operator<<(std::ostream &os, source_position pos)
{
    return os << pos.to_string();
}

#endif // !C_COMPILER_TOKEN_H
