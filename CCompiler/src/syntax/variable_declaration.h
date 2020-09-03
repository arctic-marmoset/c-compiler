#ifndef C_COMPILER_VARIABLE_DECLARATION_H
#define C_COMPILER_VARIABLE_DECLARATION_H

#include "syntax/declaration.h"

class variable_declaration : public declaration
{
public:
    variable_declaration(const token &type_specifier,
                         const token &identifier,
                         std::unique_ptr<expression> initializer = nullptr)
        : declaration(type_specifier)
        , type_specifier_(type_specifier)
        , identifier_(identifier)
        , initializer_(std::move(initializer))
    {
        if (initializer_)
        {
            children_.push_back(initializer_.get());
        }
    }

    syntax_type type() const override
    {
        return syntax_type::variable_declaration;
    }

    std::string to_string() const override
    {
        std::ostringstream ss;

        const auto &pos = trigger_token().pos;

        ss << "variable_declaration"      " "
              + pos.to_string("<", ">") + " "
              + identifier_.get().text  + " "
                "'" + type_specifier_.get().text + "'";

        if (initializer_)
        {
            ss << " cinit";
        }

        return ss.str();
    }

private:
    const_reference<token> type_specifier_;
    const_reference<token> identifier_;
    std::unique_ptr<expression> initializer_;
};

#endif
