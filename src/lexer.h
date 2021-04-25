#ifndef C_COMPILER_LEXER_H
#define C_COMPILER_LEXER_H

#include "definitions.h"
#include "token.h"
#include "token_type.h"

#include <sstream>
#include <vector>

namespace cc {

class lexer
{
public:
    explicit lexer(std::string_view text)
        : source_(text)
        , index_(0)
        , line_(1)
        , column_(1)
        , start_column_(column_)
    {
    }

    std::vector<cc::token> lex_contents();

private:
    char current() const
    {
        if (index_ >= source_.size())
        {
            return cc::chardefs::eof;
        }
        return source_[index_];
    }

    void advance()
    {
        index_++;
        column_++;
    }

    void consume()
    {
        buffer_ << current();
        advance();
    }

    void handle_newline()
    {
        advance();
        line_++;
        column_ = 1;
    }

    void skip_space();

    // TODO: Read line comment
    // TODO: Read multiline comment
    // TODO: Read char literal
    cc::token next_token();
    cc::token read_string();
    cc::token read_escaped();
    cc::token read_identifier();
    cc::token read_integer();
    cc::token read_double();
    cc::token read_float();
    cc::token read_exponent();
    cc::token read_unknown();

    cc::token_type get_keyword_type() const;

    cc::token create_token(cc::token_type type)
    {
        const std::string text = buffer_.str();
        buffer_.str(std::string());
        buffer_.clear();
        return {
            .type = type,
            .text = text,
            .pos = { line_, start_column_ },
        };
    }

private:
    std::ostringstream buffer_;

    // TODO: Should this be an std::string instead?
    std::string_view source_;
    std::size_t index_;
    std::size_t line_;
    std::size_t column_;
    std::size_t start_column_;
};

}

#endif
