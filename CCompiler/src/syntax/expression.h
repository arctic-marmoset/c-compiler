#ifndef C_COMPILER_EXPRESSION_H
#define C_COMPILER_EXPRESSION_H

#include "syntax/statement.h"

class expression : public statement
{
public:
    ~expression() = default;

    expression(const expression &) = delete;
    expression(expression &&) = delete;
    expression &operator=(const expression &) = delete;
    expression &operator=(expression &&) = delete;

protected:
    expression(const token &trigger_token)
        : statement(trigger_token)
    {
    }
};

#endif
