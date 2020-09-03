#ifndef C_COMPILER_COMPOUND_STATEMENT_H
#define C_COMPILER_COMPOUND_STATEMENT_H

#include "statement.h"

class compound_statement : public statement
{
public:
    compound_statement(const token &trigger_token)
        : statement(trigger_token)
        , has_return_(false)
    {
    }

    void add_statement(std::unique_ptr<syntax_node> statement)
    {
        statements_.emplace_back(std::move(statement));
        children_.push_back(statements_.back().get());
    }

    syntax_type type() const override
    {
        return syntax_type::compound_statement;
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

    void set_has_return(bool has_return)
    {
        has_return_ = has_return;
    }

private:
    std::vector<std::unique_ptr<syntax_node>> statements_;
    bool has_return_;
};

#endif
