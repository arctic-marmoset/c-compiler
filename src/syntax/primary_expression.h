#ifndef C_COMPILER_PRIMARY_EXPRESSION_H
#define C_COMPILER_PRIMARY_EXPRESSION_H

#include "token.h"
#include "syntax/expression.h"

namespace cc {

class primary_expression : public cc::expression
{
public:
    ~primary_expression() override = default;

    primary_expression(const primary_expression &) = delete;
    primary_expression(primary_expression &&) = delete;
    primary_expression &operator=(const primary_expression &) = delete;
    primary_expression &operator=(primary_expression &&) = delete;

protected:
    explicit primary_expression(const cc::token &trigger_token)
        : expression(trigger_token)
    {
    }
};

}

#endif
