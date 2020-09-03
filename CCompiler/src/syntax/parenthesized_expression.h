#ifndef C_COMPILER_PARENTHESIZED_EXPRESSION_H
#define C_COMPILER_PARENTHESIZED_EXPRESSION_H

#include "primary_expression.h"

class parenthesized_expression : public primary_expression
{
public:
    parenthesized_expression(const token &trigger_token,
                             std::unique_ptr<expression> enclosed_expression)
        : primary_expression(trigger_token)
        , enclosed_expression_(std::move(enclosed_expression))
    {
        children_.push_back(enclosed_expression_.get());
    }

    syntax_type type() const override
    {
        return syntax_type::parenthesized_expression;
    }

    std::string to_string() const override
    {
        const auto &pos = trigger_token().pos;

        return "parenthesized_expression"  " "
               + pos.to_string("<", ">");
    }

private:
    std::unique_ptr<expression> enclosed_expression_;
};

#endif
