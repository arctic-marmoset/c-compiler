#ifndef C_COMPILER_LITERAL_H
#define C_COMPILER_LITERAL_H

#include "token.h"
#include "syntax/primary_expression.h"

// Not sure if this is the best way to avoid code duplication across arithmetic types, but it works
#define DECLARE_LITERAL_SYNTAX_NODE(name, display_name)   \
    class name : public cc::primary_expression            \
    {                                                     \
    public:                                               \
        explicit name(const token &trigger_token)         \
            : cc::primary_expression(trigger_token)       \
        {                                                 \
        }                                                 \
                                                          \
        cc::syntax_type type() const override             \
        {                                                 \
            return cc::syntax_type::name;                 \
        }                                                 \
                                                          \
        std::string to_string() const override            \
        {                                                 \
            const auto &[_, text, pos] = trigger_token(); \
                                                          \
            return #name                       " "        \
                   + pos.to_string("<", ">") + " "        \
                   "'" display_name "'" " " + text;       \
        }                                                 \
    }

namespace cc {

DECLARE_LITERAL_SYNTAX_NODE(integer_literal, "int");
DECLARE_LITERAL_SYNTAX_NODE(double_literal, "double");
DECLARE_LITERAL_SYNTAX_NODE(float_literal, "float");

// Special cases for string_literal and char_literal

class string_literal : public cc::primary_expression
{
public:
    explicit string_literal(const cc::token &trigger_token)
        : cc::primary_expression(trigger_token)
    {
    }

    cc::syntax_type type() const override
    {
        return cc::syntax_type::string_literal;
    }

    std::string to_string() const override
    {
        std::ostringstream ss;

        const auto &[_, text, pos] = trigger_token();

        ss << "string_literal"             " "
              + pos.to_string("<", ">")  + " "
              "'char [" << text.size() << "]'"
              " " + text;

        return ss.str();
    }
};

class char_literal : public cc::primary_expression
{
public:
    explicit char_literal(const cc::token &trigger_token)
        : cc::primary_expression(trigger_token)
    {
    }

    cc::syntax_type type() const override
    {
        return cc::syntax_type::char_literal;
    }

    std::string to_string() const override
    {
        std::ostringstream ss;

        const auto &[_, text, pos] = trigger_token();

        // TODO: This is a placeholder
        ss << "char_literal"               " "
              + pos.to_string("<", ">")  + " "
              + text;

        return ss.str();
    }
};

}

// Undef to avoid polluting global namespace
#undef DECLARE_LITERAL_SYNTAX_NODE

#endif
