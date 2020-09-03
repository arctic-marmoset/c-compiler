#ifndef C_COMPILER_PRIMARY_EXPRESSION_H
#define C_COMPILER_PRIMARY_EXPRESSION_H

#include "expression.h"

class primary_expression : public expression
{
public:
    ~primary_expression() = default;

    primary_expression(const primary_expression &) = delete;
    primary_expression(primary_expression &&) = delete;
    primary_expression &operator=(const primary_expression &) = delete;
    primary_expression &operator=(primary_expression &&) = delete;

protected:
    primary_expression(const token &trigger_token)
        : expression(trigger_token)
    {
    }
};

#endif
