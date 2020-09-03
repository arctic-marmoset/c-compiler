#ifndef C_COMPILER_DECLARATION_H
#define C_COMPILER_DECLARATION_H

#include "syntax/syntax_node.h"

class declaration : public statement
{
public:
    declaration(const declaration &) = delete;
    declaration(declaration &&) = delete;
    declaration &operator=(const declaration &) = delete;
    declaration &operator=(declaration &&) = delete;

protected:
    declaration(const token &trigger_token)
        : statement(trigger_token)
    {
    }
};

#endif
