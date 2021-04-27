#ifndef C_COMPILER_COMPOUND_STATEMENT_H
#define C_COMPILER_COMPOUND_STATEMENT_H

#include "token.h"
#include "syntax/statement.h"
#include "syntax/syntax_type.h"

#include <memory>

namespace cc {

class compound_statement : public cc::statement
{
public:
    explicit compound_statement(const cc::token &trigger_token)
        : cc::statement(trigger_token)
        , has_return_(false)
    {
    }

    void add_statement(std::unique_ptr<cc::statement> statement)
    {
        statements_.emplace_back(std::move(statement));
        children_.push_back(statements_.back().get());
    }

    cc::syntax_type type() const override
    {
        return cc::syntax_type::compound_statement;
    }

    std::string to_string() const override
    {
        const auto &pos = trigger_token().pos;

        return "compound_statement"  " "
               + pos.to_string("<", ">");
    }

    bool returns() const
    {
        return has_return_;
    }

    void has_return(bool returns)
    {
        has_return_ = returns;
    }

private:
    std::vector<std::unique_ptr<cc::statement>> statements_;
    bool has_return_;
};

} // namespace cc

#endif
