#ifndef C_COMPILER_EXPRESSION_H
#define C_COMPILER_EXPRESSION_H

#include "token.h"
#include "syntax/statement.h"

namespace cc {

class expression : public cc::statement
{
public:
    ~expression() override = default;

    expression(const expression &) = delete;
    expression(expression &&) = delete;
    expression &operator=(const expression &) = delete;
    expression &operator=(expression &&) = delete;

protected:
    explicit expression(const cc::token &trigger_token)
        : statement(trigger_token)
    {
    }
};

}

#endif
