#ifndef C_COMPILER_DECLARATION_H
#define C_COMPILER_DECLARATION_H

#include "token.h"
#include "syntax/statement.h"
#include "syntax/syntax_node.h"

namespace cc {

class declaration : public cc::statement
{
public:
    ~declaration() override = default;

    declaration(const declaration &) = delete;
    declaration(declaration &&) = delete;
    declaration &operator=(const declaration &) = delete;
    declaration &operator=(declaration &&) = delete;

protected:
    explicit declaration(const cc::token &trigger_token)
        : cc::statement(trigger_token)
    {
    }
};

} // namespace cc

#endif
