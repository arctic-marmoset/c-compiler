#ifndef C_COMPILER_RETURN_STATEMENT_H
#define C_COMPILER_RETURN_STATEMENT_H

#include "expression.h"
#include "statement.h"

class return_statement : public statement
{
public:
    explicit return_statement(const token &trigger_token,
                              std::unique_ptr<expression> return_expression = nullptr)
        : statement(trigger_token)
        , expression_(std::move(return_expression))
    {
        if (expression_)
        {
            children_.push_back(expression_.get());
        }
    }

    syntax_type type() const override
    {
        return syntax_type::return_statement;
    }

    std::string to_string() const override
    {
        const auto &pos = trigger_token().pos;

        return "return_statement"    " "
               + pos.to_string("<", ">");
    }

private:
    std::unique_ptr<syntax_node> expression_;
};

#endif
