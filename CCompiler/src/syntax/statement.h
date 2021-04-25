#ifndef C_COMPILER_STATEMENT_H
#define C_COMPILER_STATEMENT_H

#include "token.h"
#include "syntax/syntax_node.h"

namespace cc {

class statement : public cc::syntax_node
{
public:
    ~statement() override = default;

    statement(const statement &) = delete;
    statement(statement &&) = delete;
    statement &operator=(const statement &) = delete;
    statement &operator=(statement &&) = delete;

protected:
    explicit statement(const cc::token &trigger_token)
        : syntax_node(trigger_token)
    {
    }
};

}

#endif
