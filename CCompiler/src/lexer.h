#ifndef C_COMPILER_LEXER_H
#define C_COMPILER_LEXER_H

#include "token.h"
#include "definitions.h"
#include <sstream>
#include <vector>

class lexer
{
public:
    lexer(std::string_view text)
        : source_(text)
        , index_(0)
        , token_start_(0)
    {
    }

    std::vector<token> lex_contents();

private:
    char current() const
    {
        if (index_ >= source_.size())
        {
            return chardefs::eof;
        }
        return source_[index_];
    }

    void next() { index_++; }

    void consume()
    {
        buffer_ << current();
        next();
    }

    void skip_space();

    token next_token();
    token read_string();
    token read_escaped();
    token read_identifier();
    token read_integer();
    token read_double();
    token read_float();
    token read_exponent();
    token read_unknown();

    bool is_keyword() const;

    token create_token(token_type type)
    {
        std::string text = buffer_.str();
        buffer_.str(std::string());
        buffer_.clear();
        return token(type, token_start_, text);
    }

private:
    std::ostringstream buffer_;

    // TODO: Should this be an std::string instead?
    std::string_view source_;
    std::size_t index_;

    std::size_t token_start_;
};

#endif // !C_COMPILER_LEXER_H
