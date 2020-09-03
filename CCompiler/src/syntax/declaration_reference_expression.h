#ifndef C_COMPILER_DECLARATION_REFERENCE_EXPRESSION_H
#define C_COMPILER_DECLARATION_REFERENCE_EXPRESSION_H

#include "primary_expression.h"

class declaration_reference_expression : public primary_expression
{
public:
    declaration_reference_expression(const token &trigger_token)
        : primary_expression(trigger_token)
    {
    }

    syntax_type type() const override
    {
        return syntax_type::declaration_reference_expression;
    }

    std::string to_string() const override
    {
        const auto &[_, text, pos] = trigger_token();

        return "declaration_reference_expression"  " "
               + pos.to_string("<", ">")         + " "
               "lvalue Var '" + text + "'";
    }
};

#endif
