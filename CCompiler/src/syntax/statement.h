#ifndef C_COMPILER_STATEMENT_H
#define C_COMPILER_STATEMENT_H

#include "syntax/syntax_node.h"
#include <cassert>

class return_statement : public syntax_node
{
public:
    explicit return_statement(const token &trigger_token)
        : syntax_node(trigger_token)
    {
    }

    void set_expression(std::unique_ptr<syntax_node> expression)
    {
        assert("expression_ has not yet been set" && !expression_);

        expression_ = std::move(expression);
        children_.push_back(expression_.get());
    }

    syntax_type type() const override
    {
        return syntax_type::return_statement;
    }

    std::string to_string() const override
    {
        const auto &pos = trigger_token_.get().pos;

        return "return_statement"     " " 
               + pos.to_string("<", ">");
    }

private:
    std::unique_ptr<syntax_node> expression_;
};

#endif
