#ifndef C_COMPILER_TOKEN_H
#define C_COMPILER_TOKEN_H

#include "token_type.h"
#include <cstddef>
#include <string>

class token
{
public:
    token(const token_type type, const std::size_t pos, std::string text)
        : type(type)
        , text(std::move(text))
        , pos(pos)
    {
    }

public:
    const token_type type;
    const std::string text;
    const std::size_t pos;
};

#endif // !C_COMPILER_TOKEN_H
