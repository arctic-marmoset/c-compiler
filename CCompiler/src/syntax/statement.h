#ifndef C_COMPILER_STATEMENT_H
#define C_COMPILER_STATEMENT_H

#include "syntax/syntax_node.h"
#include <cassert>

class statement : public syntax_node
{
public:
    ~statement() = default;

    statement(const statement &) = delete;
    statement(statement &&) = delete;
    statement &operator=(const statement &) = delete;
    statement &operator=(statement &&) = delete;

protected:
    statement(const token &trigger_token)
        : syntax_node(trigger_token)
    {
    }
};

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

class compound_statement : public statement
{
public:
    compound_statement(const token &trigger_token)
        : statement(trigger_token)
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

private:
    std::vector<std::unique_ptr<syntax_node>> statements_;
};

#endif
