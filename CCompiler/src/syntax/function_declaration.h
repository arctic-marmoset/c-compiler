#ifndef C_COMPILER_FUNCTION_DECLARATION_H
#define C_COMPILER_FUNCTION_DECLARATION_H

#include "syntax/compound_statement.h"
#include "syntax/declaration.h"

class function_declaration : public declaration
{
public:
    function_declaration(const token &type_specifier,
                         const token &identifier,
                         std::unique_ptr<compound_statement> definition = nullptr,
                         bool is_redeclared = false)
        : declaration(type_specifier)
        , type_specifier_(type_specifier)
        , identifier_(identifier)
        , definition_(std::move(definition))
        , is_redeclared_(is_redeclared)
    {
        if (definition_)
        {
            children_.push_back(definition_.get());
        }
    }

    syntax_type type() const override
    {
        return syntax_type::function_declaration;
    }

    std::string to_string() const override
    {
        std::ostringstream ss;

        const auto &pos = trigger_token_.get().pos;

        ss << "function_declaration"     " ";

        if (is_redeclared_)
        {
            ss << "prev ";
        }

        ss << pos.to_string("<", ">")          + " " 
            + identifier_.get().text           + " "
              "'" + type_specifier_.get().text + " "
              "(";

        // TODO: Add parameters to ss

        ss << ")'";

        return ss.str();
    }

private:
    const_reference<token> type_specifier_;
    const_reference<token> identifier_;
    std::unique_ptr<compound_statement> definition_;
    bool is_redeclared_;
};

#endif
