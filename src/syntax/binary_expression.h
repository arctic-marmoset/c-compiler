#ifndef C_COMPILER_BINARY_EXPRESSION_H
#define C_COMPILER_BINARY_EXPRESSION_H

#include "token.h"
#include "syntax/expression.h"
#include "syntax/syntax_type.h"

#include <utility>

namespace cc {

class binary_expression : public cc::expression
{
public:
    binary_expression(cc::token op,
                      std::unique_ptr<cc::expression> left,
                      std::unique_ptr<cc::expression> right)
        : cc::expression(left->trigger_token())
        , operator_(std::move(op))
        , left_(std::move(left))
        , right_(std::move(right))
    {
        children_.push_back(left_.get());
        children_.push_back(right_.get());
    }

    cc::syntax_type type() const override
    {
        return cc::syntax_type::binary_expression;
    }

    std::string to_string() const override
    {
        const auto &pos = trigger_token().pos;

        return "binary_expression"         " "
               + pos.to_string("<", ">") + " "
               "'" + operator_.text      + "'";
    }

private:
    cc::token operator_;
    std::unique_ptr<cc::expression> left_;
    std::unique_ptr<cc::expression> right_;
};

} // namespace cc

#endif
