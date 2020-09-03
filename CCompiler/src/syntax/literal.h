#ifndef C_COMPILER_LITERAL_H
#define C_COMPILER_LITERAL_H

#include "primary_expression.h"

// Not sure if this is the best way to avoid code duplication 
// across arithmetic types, but it works

#define DECLARE_LITERAL_SYNTAX_NODE(name, display_name)      \
    class name : public primary_expression                   \
    {                                                        \
    public:                                                  \
        explicit name(const token &trigger_token)            \
            : primary_expression(trigger_token)              \
        {                                                    \
        }                                                    \
                                                             \
        syntax_type type() const override                    \
        {                                                    \
            return syntax_type::name;                        \
        }                                                    \
                                                             \
        std::string to_string() const override               \
        {                                                    \
            const auto &[_, text, pos] = trigger_token();    \
                                                             \
            return #name                       " "           \
                   + pos.to_string("<", ">") + " "           \
                   "'" display_name "'" " " + text;          \
        }                                                    \
    }                                                        \

DECLARE_LITERAL_SYNTAX_NODE(integer_literal, "int");
DECLARE_LITERAL_SYNTAX_NODE(double_literal, "double");
DECLARE_LITERAL_SYNTAX_NODE(float_literal, "float");

// Special case for string_literal

class string_literal : public primary_expression
{
public:
    explicit string_literal(const token &trigger_token)
        : primary_expression(trigger_token)
    {
    }

    syntax_type type() const override
    {
        return syntax_type::string_literal;
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

#endif
