#ifndef C_COMPILER_TRANSLATION_UNIT_H
#define C_COMPILER_TRANSLATION_UNIT_H

#include "token.h"
#include "syntax/declaration.h"
#include "syntax/syntax_node.h"
#include "syntax/syntax_type.h"

#include <memory>

namespace cc {

class translation_unit_declaration : public cc::syntax_node
{
public:
    translation_unit_declaration(const cc::token &trigger_token,
                                 std::vector<std::unique_ptr<cc::declaration>> declarations)
        : cc::syntax_node(trigger_token)
        , declarations_(std::move(declarations))
    {
        for (const auto &decl : declarations_)
        {
            children_.push_back(decl.get());
        }
    }

    cc::syntax_type type() const override
    {
        return cc::syntax_type::translation_unit_declaration;
    }

    std::string to_string() const override
    {
        return "translation_unit_declaration";
    }

private:
    std::vector<std::unique_ptr<cc::declaration>> declarations_;
};

} // namespace cc

#endif
