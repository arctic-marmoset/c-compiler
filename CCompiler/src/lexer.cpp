#include "lexer.h"
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

token lexer::next_token()
{
    // Cache starting column of token
    start_column_ = column_;

    // Cache first char
    const char first_char = current();

    // Handle newline
    if (first_char == chardefs::cr)
    {
        read_newline();
        return next_token();
    }

    // Nothing to lex if current() == eof
    if (first_char == chardefs::eof)
    {
        return create_token(token_type::eof);
    }

    // Look for keyword or literal
    if (first_char == chardefs::quote)
    {
        consume();
        return read_string();
    }
    if (std::isalpha(first_char) || first_char == chardefs::underscore)
    {
        return read_identifier();
    }
    if (std::isdigit(first_char))
    {
        return read_integer();
    }
    if (std::isspace(first_char))
    {
        skip_space();
        return next_token();
    }

    // Not keyword nor literal, so look for operator or separator
    switch (first_char)
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
            return create_token(token_type::open_parenthesis);
        }
    case chardefs::close_paren:
        {
            consume();
            return create_token(token_type::close_parenthesis);
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
    case chardefs::open_angle:
        {
            consume();
            return create_token(token_type::open_angle);
        }
    case chardefs::close_angle:
        {
            consume();
            return create_token(token_type::close_angle);
        }
    case chardefs::open_square:
        {
            consume();
            return create_token(token_type::open_square);
        }
    case chardefs::close_square:
        {
            consume();
            return create_token(token_type::close_square);
        }
    case chardefs::comma:
        {
            consume();
            return create_token(token_type::comma);
        }
    case chardefs::semicolon:
        {
            consume();
            return create_token(token_type::semicolon);
        }
    default:
        break;
    }

    // Could not identify the token
    return read_unknown();
}

token lexer::read_string()
{
    // TODO: Handle missing closing quote gracefully

    while (current() != chardefs::quote &&
           current() != chardefs::backslash &&
           current() != chardefs::eof)
    {
        consume();
    }

    const char breaking_char = current();

    switch (breaking_char)
    {
    case chardefs::quote:
        {
            consume();
            break;
        }
    case chardefs::backslash:
        {
            consume();
            return read_escaped();
        }
    case chardefs::eof:
        {
            return create_token(token_type::invalid);
        }
    }

    return create_token(token_type::string_literal);
}

token lexer::read_escaped()
{
    const char first_char = current();

    // TODO: Handle octal and hex

    switch (first_char)
    {
    case '\'':
    case '\"':
    case '\?':
    case '\\':
    case '\a':
    case '\b':
    case '\f':
    case '\n':
    case '\r':
    case '\t':
    case '\v':
        consume();
    }

    // If no cases matched, ignore backslash

    return read_string();
}

token lexer::read_identifier()
{
    while (std::isalnum(current()) || current() == chardefs::underscore)
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
    while (std::isspace(current()))
    {
        advance();
    }
}

token lexer::read_integer()
{
    while (std::isdigit(current()))
    {
        consume();
    }

    // A non-digit char was hit
    const char breaking_char = current();

    // Handle special cases
    switch (breaking_char)
    {
    case chardefs::period:
        {
            consume();
            return read_double();
        }
    case 'e':
        {
            consume();
            return read_exponent();
        }
    }

    return create_token(token_type::integer_literal);
}

token lexer::read_double()
{
    // Keep track of mantissa length
    std::size_t mantissa_length = 0;

    while (std::isdigit(current()))
    {
        mantissa_length++;
        consume();
    }

    // A double must have a mantissa
    if (mantissa_length == 0)
    {
        return read_unknown();
    }

    // A non-digit char was hit
    const char breaking_char = current();

    // Handle special cases
    switch (breaking_char)
    {
    case chardefs::period:
        {
            return read_unknown();
        }
    case 'e':
        {
            consume();
            return read_exponent();
        }
    case 'f':
        {
            consume();
            return read_float();
        }
    }

    return create_token(token_type::double_literal);
}

token lexer::read_float()
{
    // This method is only called when current() == 'f',
    // at which point the float token is complete.
    return create_token(token_type::float_literal);
}

token lexer::read_exponent()
{
    // Consume an optional '+' or '-' character
    if (current() == chardefs::plus || current() == chardefs::dash)
    {
        consume();
    }

    std::size_t exponent_length = 0;
    while (std::isdigit(current()))
    {
        exponent_length++;
        consume();
    }

    // An exponent must have at least 1 digit
    if (exponent_length == 0)
    {
        return read_unknown();
    }

    // Check for float suffix
    if (current() == 'f')
    {
        consume();
        return read_float();
    }

    return create_token(token_type::double_literal);
}

token lexer::read_unknown()
{
    while (!std::isspace(current()) && current() != chardefs::eof)
    {
        consume();
    }
    return create_token(token_type::invalid);
}

bool lexer::is_keyword() const
{
    // TODO: Maybe automate this
    const std::string text = buffer_.str();
    return (text == keyworddefs::char_keyword     ||
            text == keyworddefs::int_keyword      ||
            text == keyworddefs::double_keyword   ||
            text == keyworddefs::float_keyword    ||
            text == keyworddefs::struct_keyword   ||
            text == keyworddefs::enum_keyword     ||
            text == keyworddefs::void_keyword     ||
            text == keyworddefs::short_keyword    ||
            text == keyworddefs::long_keyword     ||
            text == keyworddefs::const_keyword    ||
            text == keyworddefs::static_keyword   ||
            text == keyworddefs::if_keyword       ||
            text == keyworddefs::else_keyword     ||
            text == keyworddefs::for_keyword      ||
            text == keyworddefs::while_keyword    ||
            text == keyworddefs::break_keyword    ||
            text == keyworddefs::continue_keyword ||
            text == keyworddefs::return_keyword);
}
