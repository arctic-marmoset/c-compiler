#ifndef C_COMPILER_PARENTHESIZED_EXPRESSION_H
#define C_COMPILER_PARENTHESIZED_EXPRESSION_H

#include "token.h"
#include "syntax/expression.h"
#include "syntax/primary_expression.h"
#include "syntax/syntax_type.h"

namespace cc {

class parenthesized_expression : public cc::primary_expression
{
public:
    parenthesized_expression(const cc::token &trigger_token,
                             std::unique_ptr<cc::expression> enclosed_expression)
        : primary_expression(trigger_token)
        , enclosed_expression_(std::move(enclosed_expression))
    {
        children_.push_back(enclosed_expression_.get());
    }

    cc::syntax_type type() const override
    {
        return cc::syntax_type::parenthesized_expression;
    }

    std::string to_string() const override
    {
        const auto &pos = trigger_token().pos;

        return "parenthesized_expression"  " "
               + pos.to_string("<", ">");
    }

private:
    std::unique_ptr<cc::expression> enclosed_expression_;
};

}

#endif
