#ifndef C_COMPILER_PARSER_H
#define C_COMPILER_PARSER_H

#include "syntax/expression.h"
#include "syntax/statement.h"
#include "syntax/syntax_node.h"
#include "token.h"
#include <vector>

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
        return parse_compound_statement();
    }

private:
    const token& current_token() const
    {
        return tokens_.get()[index_];
    }

    void advance()
    {
        if (index_ + 1 < tokens_.get().size())
        {
            index_++;
        }
    }

    /**
     * @brief Asserts that the current token is of the specified `type`,
     *        and advances to the next token if true.
     *
     * @param[in] type The type that the current token should match.
     * @return         `true` if the assertion passed. `false` otherwise.
     */
    bool expect(token_type type)
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

    std::unique_ptr<primary_expression>       parse_literal();
    std::unique_ptr<parenthesized_expression> parse_parenthesized_expression();
    std::unique_ptr<primary_expression>       parse_primary_expression();
    std::unique_ptr<return_statement>         parse_return_statement();
    std::unique_ptr<compound_statement>       parse_compound_statement();

private:
    std::size_t index_;
    const_reference<std::vector<token>> tokens_;
};

#endif
