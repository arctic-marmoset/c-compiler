#ifndef C_COMPILER_LEXER_H
#define C_COMPILER_LEXER_H

#include "token.h"

#include <sstream>
#include <vector>

class lexer
{
public:
    lexer(std::string text)
        : source_(std::move(text))
        , index_(0)
        , token_start_(0)
    {
    }

    std::vector<token> lex_contents();

private:
    char current();
    void next();

    void consume();
    void skip_space();

    token next_token();
    token read_identifier();
    token read_integer();
    token read_double();
    token read_float();
    token read_exponent();
    token read_unknown();

    bool is_keyword();

    token create_token(token_type type);

private:
    std::ostringstream buffer_;
    std::string source_;
    std::size_t index_;

    std::size_t token_start_;
};

#endif // !C_COMPILER_LEXER_H
