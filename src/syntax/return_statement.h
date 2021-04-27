#ifndef C_COMPILER_RETURN_STATEMENT_H
#define C_COMPILER_RETURN_STATEMENT_H

#include "token.h"
#include "syntax/expression.h"
#include "syntax/statement.h"
#include "syntax/syntax_type.h"

namespace cc {

class return_statement : public cc::statement
{
public:
    explicit return_statement(const cc::token &trigger_token,
                              std::unique_ptr<expression> return_expression = nullptr)
        : cc::statement(trigger_token)
        , expression_(std::move(return_expression))
    {
        if (expression_)
        {
            children_.push_back(expression_.get());
        }
    }

    cc::syntax_type type() const override
    {
        return cc::syntax_type::return_statement;
    }

    std::string to_string() const override
    {
        const auto &pos = trigger_token().pos;

        return "return_statement"    " "
               + pos.to_string("<", ">");
    }

    const cc::expression *return_expression() const
    {
        return expression_.get();
    }

private:
    std::unique_ptr<cc::expression> expression_;
};

} // namespace cc

#endif
