#ifndef C_COMPILER_PARSER_H
#define C_COMPILER_PARSER_H

#include "symbol_table.h"
#include "syntax/compound_statement.h"
#include "syntax/translation_unit_declaration.h"
#include "token.h"
#include <vector>

class syntax_node;
class declaration;
class expression;
class primary_expression;
class binary_expression;
class parenthesized_expression;
class variable_declaration;
class function_declaration;
class declaration_reference_expression;
class return_statement;

// TODO: Implement better error handling and reporting

class parser
{
    template <typename T>
    using const_reference = std::reference_wrapper<const T>;

public:
    explicit parser(const std::vector<token> &tokens)
        : index_(0)
        , tokens_(tokens)
    {
    }

    std::unique_ptr<syntax_node> parse_contents()
    {
        return parse_translation_unit();
    }

private:
    const token &current_token() const
    {
        return tokens_.get()[index_];
    }

    const token &peek_token(std::size_t lookahead)
    {
        const auto peekIndex = index_ + lookahead;
        if (peekIndex <= tokens_.get().size())
        {
            return tokens_.get()[peekIndex];
        }
        else
        {
            return tokens_.get().back();
        }
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
    bool consume(token_type type)
    {
        const bool matched = match(type);

        if (matched)
        {
            advance();
        }

        return matched;
    }

    /**
     * @brief Matches the current token against a single `token_type`.
     *
     * @param[in] first The `token_type` to match.
     * @return          `true` if matches. `false` otherwise.
     */
    bool match(const token_type &first) const
    {
        return current_token().type == first;
    }

    template <typename... Ts>
    using are_token_types = std::conjunction<std::is_same<Ts, token_type>...>;

    /**
     * @brief Matches the current token against a variable number of `token_type` arguments.
     *
     * @param[in] first The first token to match.
     * @param[in] rest  The remaining tokens to match.
     * @return    `true` if at least one of the arguments matches. `false` otherwise.
     */
    template <typename... Args, typename = std::enable_if_t<are_token_types<Args...>::value>>
    bool match(const token_type &first, const Args &... rest) const
    {
        return match(first) || match(rest...);
    }

    std::unique_ptr<variable_declaration>             parse_variable_declaration(const token &type_specifier, const token &identifier);
    std::unique_ptr<function_declaration>             parse_function_declaration(const token &type_specifier, const token &identifier);
    std::unique_ptr<declaration>                      parse_declaration();
    std::unique_ptr<primary_expression>               parse_literal();
    std::unique_ptr<parenthesized_expression>         parse_parenthesized_expression();
    std::unique_ptr<declaration_reference_expression> parse_declaration_reference_expression();
    std::unique_ptr<primary_expression>               parse_primary_expression();
    std::unique_ptr<binary_expression>                parse_binary_expression();
    std::unique_ptr<expression>                       parse_expression();
    std::unique_ptr<return_statement>                 parse_return_statement();
    std::unique_ptr<compound_statement>               parse_compound_statement();
    std::unique_ptr<statement>                        parse_expression_statement();
    std::unique_ptr<statement>                        parse_statement();
    std::unique_ptr<translation_unit_declaration>     parse_translation_unit();

private:
    std::size_t index_;
    const_reference<std::vector<token>> tokens_;
    symbol_table symbols_;
};

#endif
