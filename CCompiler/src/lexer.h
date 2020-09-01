#ifndef C_COMPILER_LEXER_H
#define C_COMPILER_LEXER_H

#include "token.h"
#include "definitions.h"
#include <sstream>
#include <vector>

class lexer
{
public:
    explicit lexer(const std::string_view text)
        : source_(text)
        , index_(0)
        , line_(1)
        , column_(1)
        , start_column_(column_)
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

    // TODO: This naively assumes newline == \r\n
    void handle_newline()
    {
        advance();
        line_++;
        column_ = 1;
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

    token_type get_keyword_type() const;

    token create_token(const token_type type)
    {
        const std::string text = buffer_.str();
        buffer_.str(std::string());
        buffer_.clear();
        return token(type, line_, start_column_, text);
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

#endif // !C_COMPILER_LEXER_H
