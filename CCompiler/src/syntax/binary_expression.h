#ifndef C_COMPILER_BINARY_EXPRESSION_H
#define C_COMPILER_BINARY_EXPRESSION_H

#include "expression.h"

class binary_expression : public expression
{
public:
    binary_expression(std::unique_ptr<expression> left,
                      const token &op,
                      std::unique_ptr<expression> right)
        : expression(left->trigger_token())
        , left_(std::move(left))
        , right_(std::move(right))
        , operator_(op)
    {
        children_.push_back(left_.get());
        children_.push_back(right_.get());
    }

    syntax_type type() const override
    {
        return syntax_type::binary_expression;
    }

    std::string to_string() const override
    {
        const auto &pos = trigger_token().pos;

        return "binary_expression"          " "
               + pos.to_string("<", ">")  + " "
               "'" + operator_.get().text + "'";
    }

private:
    std::unique_ptr<expression> left_;
    std::unique_ptr<expression> right_;
    const_reference<token> operator_;
};

#endif
