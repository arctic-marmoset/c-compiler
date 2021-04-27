#ifndef C_COMPILER_FUNCTION_DECLARATION_H
#define C_COMPILER_FUNCTION_DECLARATION_H

#include "token.h"
#include "syntax/compound_statement.h"
#include "syntax/declaration.h"
#include "syntax/syntax_type.h"

#include <utility>

namespace cc {

class function_declaration : public cc::declaration
{
public:
    function_declaration(const cc::token &type_specifier,
                         cc::token identifier,
                         std::unique_ptr<cc::compound_statement> definition = nullptr,
                         bool is_redeclared = false)
        : cc::declaration(type_specifier)
        , type_specifier_(type_specifier)
        , identifier_(std::move(identifier))
        , definition_(std::move(definition))
        , is_redeclared_(is_redeclared)
    {
        if (definition_)
        {
            children_.push_back(definition_.get());
        }
    }

    cc::syntax_type type() const override
    {
        return cc::syntax_type::function_declaration;
    }

    std::string to_string() const override
    {
        std::ostringstream ss;

        const auto &pos = trigger_token().pos;

        ss << "function_declaration ";

        if (is_redeclared_)
        {
            ss << "prev ";
        }

        ss << pos.to_string("<", ">")    + " "
            + identifier_.text           + " "
              "'" + type_specifier_.text + " "
              "(";

        // TODO: Add parameters to ss

        ss << ")'";

        return ss.str();
    }

    std::string identifier() const
    {
        return identifier_.text;
    }

    const std::unique_ptr<cc::compound_statement> &definition() const
    {
        return definition_;
    }

private:
    cc::token type_specifier_;
    cc::token identifier_;
    std::unique_ptr<cc::compound_statement> definition_;
    bool is_redeclared_;
};

} // namespace cc

#endif
