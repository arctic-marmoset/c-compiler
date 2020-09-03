#ifndef C_COMPILER_TRANSLATION_UNIT_H
#define C_COMPILER_TRANSLATION_UNIT_H

#include "syntax/declaration.h"

class translation_unit_declaration : public syntax_node
{
public:
    translation_unit_declaration(const token &trigger_token,
                                 std::vector<std::unique_ptr<declaration>> declarations)
        : syntax_node(trigger_token)
        , declarations_(std::move(declarations))
    {
        for (const auto &decl : declarations_)
        {
            children_.push_back(decl.get());
        }
    }

    syntax_type type() const override
    {
        return syntax_type::translation_unit_declaration;
    }

    std::string to_string() const override
    {
        return "translation_unit_declaration";
    }

private:
    std::vector<std::unique_ptr<declaration>> declarations_;
};

#endif
