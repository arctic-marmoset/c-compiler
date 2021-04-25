#include "lexer.h"
#include "token.h"
#include "token_type.h"

#include <cctype>
#include <unordered_map>

std::vector<cc::token> cc::lexer::lex_contents()
{
    std::vector<cc::token> tokens;

    while (current() != cc::chardefs::eof)
    {
        tokens.push_back(next_token());
    }

    return tokens;
}

cc::token cc::lexer::next_token()
{
    // Cache starting column of token
    start_column_ = column_;

    // Cache first char
    const char first_char = current();

    // Skip CR since no modern OS uses CR for newline
    if (first_char == cc::chardefs::cr)
    {
        advance();
        return next_token();
    }

    if (first_char == cc::chardefs::lf)
    {
        handle_newline();
        return next_token();
    }

    // Nothing to lex if current() == eof
    if (first_char == cc::chardefs::eof)
    {
        return create_token(cc::token_type::eof);
    }

    // Look for keyword or literal
    if (first_char == cc::chardefs::quote)
    {
        consume();
        return read_string();
    }
    if (std::isalpha(first_char) || first_char == cc::chardefs::underscore)
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
    case cc::chardefs::plus:
        {
            consume();
            return create_token(cc::token_type::plus);
        }
    case cc::chardefs::dash:
        {
            consume();
            return create_token(cc::token_type::minus);
        }
    case cc::chardefs::asterisk:
        {
            consume();
            return create_token(cc::token_type::asterisk);
        }
    case cc::chardefs::forward_slash:
        {
            consume();
            return create_token(cc::token_type::forward_slash);
        }
    case cc::chardefs::equal:
        {
            consume();
            return create_token(cc::token_type::assign);
        }
    case cc::chardefs::open_paren:
        {
            consume();
            return create_token(cc::token_type::open_parenthesis);
        }
    case cc::chardefs::close_paren:
        {
            consume();
            return create_token(cc::token_type::close_parenthesis);
        }
    case cc::chardefs::open_brace:
        {
            consume();
            return create_token(cc::token_type::open_brace);
        }
    case cc::chardefs::close_brace:
        {
            consume();
            return create_token(cc::token_type::close_brace);
        }
    case cc::chardefs::open_angle:
        {
            consume();
            return create_token(cc::token_type::open_angle);
        }
    case cc::chardefs::close_angle:
        {
            consume();
            return create_token(cc::token_type::close_angle);
        }
    case cc::chardefs::open_square:
        {
            consume();
            return create_token(cc::token_type::open_square);
        }
    case cc::chardefs::close_square:
        {
            consume();
            return create_token(cc::token_type::close_square);
        }
    case cc::chardefs::comma:
        {
            consume();
            return create_token(cc::token_type::comma);
        }
    case cc::chardefs::semicolon:
        {
            consume();
            return create_token(cc::token_type::semicolon);
        }
    default:
        break;
    }

    // Could not identify the token
    return read_unknown();
}

cc::token cc::lexer::read_string()
{
    // TODO: Report missing closing quote as error

    while (current() != cc::chardefs::quote &&
           current() != cc::chardefs::backslash &&
           current() != cc::chardefs::cr &&
           current() != cc::chardefs::lf &&
           current() != cc::chardefs::eof)
    {
        consume();
    }

    const char breaking_char = current();

    switch (breaking_char)
    {
    case cc::chardefs::cr:
        {
            advance();
            return read_string();
        }
    case cc::chardefs::lf:
        {
            handle_newline();
            break;
        }
    case cc::chardefs::quote:
        {
            consume();
            break;
        }
    case cc::chardefs::backslash:
        {
            return read_escaped();
        }
    case cc::chardefs::eof:
        {
            return create_token(cc::token_type::unknown);
        }
    }

    return create_token(cc::token_type::string_literal);
}

cc::token cc::lexer::read_escaped()
{
    // Skip the backslash for now. We will determine
    // whether it should be part of the string later.
    advance();

    const char first_char = current();

    // TODO: Handle octal and hex

    switch (first_char)
    {
    case cc::chardefs::single_quote:
    case cc::chardefs::double_quote:
    case cc::chardefs::question:
    case cc::chardefs::back_slash:
    case 'a':
    case 'b':
    case 'f':
    case 'n':
    case 'r':
    case 't':
    case 'v':
        {
            buffer_ << cc::chardefs::back_slash;
            consume();
            break;
        }
    case cc::chardefs::cr:
        {
            advance();
            return read_escaped();
        }
    case cc::chardefs::lf:
        {
            handle_newline();
            break;
        }
    }

    // If no cases matched, ignore backslash

    return read_string();
}

cc::token cc::lexer::read_identifier()
{
    while (std::isalnum(current()) || current() == cc::chardefs::underscore)
    {
        consume();
    }

    if (const auto type = get_keyword_type(); type != cc::token_type::unknown)
    {
        return create_token(type);
    }

    return create_token(cc::token_type::identifier);
}

void cc::lexer::skip_space()
{
    while (std::isspace(current()))
    {
        advance();
    }
}

cc::token cc::lexer::read_integer()
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
    case cc::chardefs::period:
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

    return create_token(cc::token_type::integer_literal);
}

cc::token cc::lexer::read_double()
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
    case cc::chardefs::period:
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

    return create_token(cc::token_type::double_literal);
}

cc::token cc::lexer::read_float()
{
    // This method is only called when current() == 'f',
    // at which point the float token is complete.
    return create_token(cc::token_type::float_literal);
}

cc::token cc::lexer::read_exponent()
{
    // Consume an optional '+' or '-' character
    if (current() == cc::chardefs::plus || current() == cc::chardefs::dash)
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

    return create_token(cc::token_type::double_literal);
}

cc::token cc::lexer::read_unknown()
{
    while (!std::isspace(current()) && current() != cc::chardefs::eof)
    {
        consume();
    }
    return create_token(cc::token_type::unknown);
}

// TODO: Is there a better place to put this?

static const std::unordered_map<std::string_view, cc::token_type> keyword_types =
{
    { cc::keyworddefs::char_keyword,     cc::token_type::char_keyword     },
    { cc::keyworddefs::int_keyword,      cc::token_type::int_keyword      },
    { cc::keyworddefs::double_keyword,   cc::token_type::double_keyword   },
    { cc::keyworddefs::float_keyword,    cc::token_type::float_keyword    },
    { cc::keyworddefs::struct_keyword,   cc::token_type::struct_keyword   },
    { cc::keyworddefs::enum_keyword,     cc::token_type::enum_keyword     },
    { cc::keyworddefs::void_keyword,     cc::token_type::void_keyword     },
    { cc::keyworddefs::short_keyword,    cc::token_type::short_keyword    },
    { cc::keyworddefs::long_keyword,     cc::token_type::long_keyword     },
    { cc::keyworddefs::const_keyword,    cc::token_type::const_keyword    },
    { cc::keyworddefs::static_keyword,   cc::token_type::static_keyword   },
    { cc::keyworddefs::if_keyword,       cc::token_type::if_keyword       },
    { cc::keyworddefs::else_keyword,     cc::token_type::else_keyword     },
    { cc::keyworddefs::for_keyword,      cc::token_type::for_keyword      },
    { cc::keyworddefs::while_keyword,    cc::token_type::while_keyword    },
    { cc::keyworddefs::break_keyword,    cc::token_type::break_keyword    },
    { cc::keyworddefs::continue_keyword, cc::token_type::continue_keyword },
    { cc::keyworddefs::return_keyword,   cc::token_type::return_keyword   },
};

cc::token_type cc::lexer::get_keyword_type() const
{
    const std::string text = buffer_.str();

    if (const auto it = keyword_types.find(text); it != keyword_types.end())
    {
        return it->second;
    }
    else
    {
        return cc::token_type::unknown;
    }
}
