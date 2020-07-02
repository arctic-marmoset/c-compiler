#ifndef C_COMPILER_TOKEN_H
#define C_COMPILER_TOKEN_H

#include "token_type.h"
#include <cstddef>
#include <string>

class token
{
public:
    token(token_type type, std::size_t pos, std::string text)
        : type(type)
        , text(std::move(text))
        , pos_(pos)
    {
    }

public:
    token_type type;
    std::string text;

private:
    std::size_t pos_;
};

#endif // !C_COMPILER_TOKEN_H
