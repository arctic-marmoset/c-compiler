#include "lexer.h"

#include "definitions.h"

#include <cctype>

std::vector<token> lexer::lex_contents()
{
    std::vector<token> tokens;

    while (current() != chardefs::eof)
    {
        tokens.push_back(next_token());
    }

    return tokens;
}

char lexer::current()
{
    if (index_ > source_.size())
    {
        return '\0';
    }
    return source_[index_];
}

void lexer::next()
{
    index_++;
}

void lexer::consume()
{
    buffer_ << current();
    next();
}

token lexer::next_token()
{
    token_start_ = index_;

    if (current() == chardefs::eof)
    {
        return create_token(token_type::eof);
    }
    if (isalpha(current()) || current() == chardefs::underscore)
    {
        return read_identifier();
    }
    if (isdigit(current()))
    {
        return read_integer();
    }
    if (isspace(current()))
    {
        skip_space();
        return next_token();
    }

    switch (current())
    {
    case chardefs::plus:
        {
            consume();
            return create_token(token_type::plus);
        }
    case chardefs::dash:
        {
            consume();
            return create_token(token_type::minus);
        }
    case chardefs::asterisk:
        {
            consume();
            return create_token(token_type::asterisk);
        }
    case chardefs::forward_slash:
        {
            consume();
            return create_token(token_type::forward_slash);
        }
    case chardefs::open_paren:
        {
            consume();
            return create_token(token_type::open_paren);
        }
    case chardefs::close_paren:
        {
            consume();
            return create_token(token_type::close_paren);
        }
    case chardefs::open_brace:
        {
            consume();
            return create_token(token_type::open_brace);
        }
    case chardefs::close_brace:
        {
            consume();
            return create_token(token_type::close_brace);
        }
    case chardefs::semicolon:
        {
            consume();
            return create_token(token_type::semicolon);
        }
    }

    return read_unknown();
}

token lexer::read_identifier()
{
    while (isalnum(current()) || current() == chardefs::underscore)
    {
        consume();
    }
    if (is_keyword())
    {
        return create_token(token_type::keyword);
    }
    return create_token(token_type::identifier);
}

void lexer::skip_space()
{
    while (isspace(current()))
    {
        next();
    }
}

token lexer::read_integer()
{
    while (isdigit(current()))
    {
        consume();
    }
    if (current() == chardefs::period)
    {
        consume();
        return read_double();
    }
    if (current() == 'e')
    {
        consume();
        return read_exponent();
    }
    return create_token(token_type::integer);
}

token lexer::read_double()
{
    std::size_t mantissa_length = 0;

    while (isdigit(current()))
    {
        mantissa_length++;
        consume();
    }

    if (mantissa_length == 0)
    {
        return read_unknown();
    }
    if (current() == chardefs::period)
    {
        return read_unknown();
    }
    if (current() == 'e')
    {
        consume();
        return read_exponent();
    }
    if (current() == 'f')
    {
        consume();
        return read_float();
    }

    return create_token(token_type::double_precision);
}

token lexer::read_float()
{
    return create_token(token_type::single_precision);
}

token lexer::read_exponent()
{
    if (current() == chardefs::plus || current() == chardefs::dash)
    {
        consume();
    }

    std::size_t exponent_length = 0;
    while (isdigit(current()))
    {
        exponent_length++;
        consume();
    }

    if (exponent_length == 0)
    {
        return read_unknown();
    }

    if (current() == 'f')
    {
        consume();
        return read_float();
    }

    return create_token(token_type::double_precision);
}

token lexer::read_unknown()
{
    while (!isspace(current()) && !current() == chardefs::eof)
    {
        consume();
    }
    return create_token(token_type::invalid);
}

bool lexer::is_keyword()
{
    // TODO: Maybe automate this
    std::string text = buffer_.str();
    return (text == keyworddefs::char_kw     ||
            text == keyworddefs::int_kw      ||
            text == keyworddefs::double_kw   ||
            text == keyworddefs::float_kw    ||
            text == keyworddefs::struct_kw   ||
            text == keyworddefs::enum_kw     ||
            text == keyworddefs::void_kw     ||
            text == keyworddefs::short_kw    ||
            text == keyworddefs::long_kw     ||
            text == keyworddefs::const_kw    ||
            text == keyworddefs::static_kw   ||
            text == keyworddefs::if_kw       ||
            text == keyworddefs::else_kw     ||
            text == keyworddefs::for_kw      ||
            text == keyworddefs::while_kw    ||
            text == keyworddefs::break_kw    ||
            text == keyworddefs::continue_kw ||
            text == keyworddefs::return_kw);
}

token lexer::create_token(token_type type)
{
    std::string text = buffer_.str();
    buffer_.str(std::string());
    buffer_.clear();
    return token(type, token_start_, text);
}
