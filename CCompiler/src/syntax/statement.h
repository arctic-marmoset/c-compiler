#ifndef C_COMPILER_STATEMENT_H
#define C_COMPILER_STATEMENT_H

#include "syntax/syntax_node.h"

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

#endif
