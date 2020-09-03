#include "lexer.h"
#include <cctype>
#include <unordered_map>

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

    // Skip CR since no modern OS uses CR for newline
    if (first_char == chardefs::cr)
    {
        advance();
        return next_token();
    }

    if (first_char == chardefs::lf)
    {
        handle_newline();
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
    case chardefs::equal:
        {
            consume();
            return create_token(token_type::assign);
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
    // TODO: Report missing closing quote as error

    while (current() != chardefs::quote &&
           current() != chardefs::backslash &&
           current() != chardefs::cr &&
           current() != chardefs::lf &&
           current() != chardefs::eof)
    {
        consume();
    }

    const char breaking_char = current();

    switch (breaking_char)
    {
    case chardefs::cr:
        {
            advance();
            return read_string();
        }
    case chardefs::lf:
        {
            handle_newline();
            break;
        }
    case chardefs::quote:
        {
            consume();
            break;
        }
    case chardefs::backslash:
        {
            return read_escaped();
        }
    case chardefs::eof:
        {
            return create_token(token_type::unknown);
        }
    }

    return create_token(token_type::string_literal);
}

token lexer::read_escaped()
{
    // Skip the backslash for now. We will determine
    // whether it should be part of the string later.
    advance();

    const char first_char = current();

    // TODO: Handle octal and hex

    switch (first_char)
    {
    case chardefs::single_quote:
    case chardefs::double_quote:
    case chardefs::question:
    case chardefs::back_slash:
    case 'a':
    case 'b':
    case 'f':
    case 'n':
    case 'r':
    case 't':
    case 'v':
        {
            buffer_ << chardefs::back_slash;
            consume();
            break;
        }
    case chardefs::cr:
        {
            advance();
            return read_escaped();
        }
    case chardefs::lf:
        {
            handle_newline();
            break;
        }
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

    if (const auto type = get_keyword_type(); type != token_type::unknown)
    {
        return create_token(type);
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
    return create_token(token_type::unknown);
}

// TODO: Is there a better place to put this?

static const std::unordered_map<std::string_view, token_type> keyword_types =
{
    { keyworddefs::char_keyword,     token_type::char_keyword     },
    { keyworddefs::int_keyword,      token_type::int_keyword      },
    { keyworddefs::double_keyword,   token_type::double_keyword   },
    { keyworddefs::float_keyword,    token_type::float_keyword    },
    { keyworddefs::struct_keyword,   token_type::struct_keyword   },
    { keyworddefs::enum_keyword,     token_type::enum_keyword     },
    { keyworddefs::void_keyword,     token_type::void_keyword     },
    { keyworddefs::short_keyword,    token_type::short_keyword    },
    { keyworddefs::long_keyword,     token_type::long_keyword     },
    { keyworddefs::const_keyword,    token_type::const_keyword    },
    { keyworddefs::static_keyword,   token_type::static_keyword   },
    { keyworddefs::if_keyword,       token_type::if_keyword       },
    { keyworddefs::else_keyword,     token_type::else_keyword     },
    { keyworddefs::for_keyword,      token_type::for_keyword      },
    { keyworddefs::while_keyword,    token_type::while_keyword    },
    { keyworddefs::break_keyword,    token_type::break_keyword    },
    { keyworddefs::continue_keyword, token_type::continue_keyword },
    { keyworddefs::return_keyword,   token_type::return_keyword   },
};

token_type lexer::get_keyword_type() const
{
    const std::string text = buffer_.str();

    if (const auto it = keyword_types.find(text); it != keyword_types.end())
    {
        return it->second;
    }
    else
    {
        return token_type::unknown;
    }
}
