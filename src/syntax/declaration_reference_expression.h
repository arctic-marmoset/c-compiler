#ifndef C_COMPILER_DECLARATION_REFERENCE_EXPRESSION_H
#define C_COMPILER_DECLARATION_REFERENCE_EXPRESSION_H

#include "token.h"
#include "syntax/primary_expression.h"
#include "syntax/syntax_type.h"

namespace cc {

class declaration_reference_expression : public cc::primary_expression
{
public:
    explicit declaration_reference_expression(const cc::token &trigger_token)
        : cc::primary_expression(trigger_token)
    {
    }

    cc::syntax_type type() const override
    {
        return cc::syntax_type::declaration_reference_expression;
    }

    std::string to_string() const override
    {
        const auto &[_, text, pos] = trigger_token();

        return "declaration_reference_expression"  " "
               + pos.to_string("<", ">")         + " "
               "lvalue Var '" + text + "'";
    }
};

} // namespace cc

#endif
