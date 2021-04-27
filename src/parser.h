#ifndef C_COMPILER_PARSER_H
#define C_COMPILER_PARSER_H

#include "symbol_table.h"
#include "token.h"
#include "token_type.h"
#include "syntax/translation_unit_declaration.h"

#include <memory>
#include <stack>
#include <vector>

namespace cc {

class binary_expression;
class compound_statement;
class declaration;
class declaration_reference_expression;
class expression;
class function_declaration;
class primary_expression;
class parenthesized_expression;
class return_statement;
class statement;
class syntax_node;
class variable_declaration;

class parser
{
    template <typename T>
    using const_reference = std::reference_wrapper<const T>;

public:
    explicit parser(const std::vector<cc::token> &tokens)
        : index_(0)
        , tokens_(tokens)
        , scope_({&symbols_})
    {
    }

    std::unique_ptr<cc::syntax_node> parse_contents()
    {
        return parse_translation_unit();
    }

private:
    const cc::token &current_token() const
    {
        return tokens_.get()[index_];
    }

    const cc::token &peek_token(std::size_t lookahead)
    {
        const auto peek_index = index_ + lookahead;

        if (peek_index <= tokens_.get().size())
        {
            return tokens_.get()[peek_index];
        }

        return tokens_.get().back();
    }

    void advance()
    {
        if (index_ <= tokens_.get().size())
        {
            index_++;
        }
    }

    /**
     * @brief Consumes a token of the specified `token_type`.
     *
     * @param[in] type The `token_type` that the current token should match.
     * @return         `true` if the current token was consumed. `false` otherwise.
     */
    bool consume(cc::token_type type)
    {
        const bool matched = match(type);

        if (matched)
        {
            advance();
        }

        return matched;
    }

    template <typename... Ts>
    using are_token_types = std::conjunction<std::is_same<Ts, cc::token_type>...>;

    /**
     * @brief Matches the current token against a variable number of `token_type` arguments.
     *
     * @param[in] types  The tokens to match.
     * @return    `true` if at least one of the arguments matches. `false` otherwise.
     */
    template <typename... Args, typename = std::enable_if_t<are_token_types<Args...>::value>>
    bool match(const Args &...types) const
    {
        return ((current_token().type == types) || ...);
    }

    // clang-format off
    std::unique_ptr<cc::variable_declaration>             parse_variable_declaration(const cc::token &type_specifier, const cc::token &identifier);
    std::unique_ptr<cc::function_declaration>             parse_function_declaration(const cc::token &type_specifier, const cc::token &identifier);
    std::unique_ptr<cc::declaration>                      parse_declaration();
    std::unique_ptr<cc::primary_expression>               parse_literal();
    std::unique_ptr<cc::parenthesized_expression>         parse_parenthesized_expression();
    std::unique_ptr<cc::declaration_reference_expression> parse_declaration_reference_expression();
    std::unique_ptr<cc::primary_expression>               parse_primary_expression();
    std::unique_ptr<cc::binary_expression>                parse_binary_expression();
    std::unique_ptr<cc::expression>                       parse_expression();
    std::unique_ptr<cc::return_statement>                 parse_return_statement();
    std::unique_ptr<cc::compound_statement>               parse_compound_statement();
    std::unique_ptr<cc::statement>                        parse_expression_statement();
    std::unique_ptr<cc::statement>                        parse_statement();
    std::unique_ptr<cc::translation_unit_declaration>     parse_translation_unit();
    // clang-format on

private:
    std::size_t index_;
    const_reference<std::vector<cc::token>> tokens_;
    cc::symbol_table symbols_;
    std::stack<cc::symbol_table *> scope_;
};

} // namespace cc

// TODO: Implement better error handling and reporting

#endif
