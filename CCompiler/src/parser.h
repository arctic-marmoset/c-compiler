#ifndef C_COMPILER_PARSER_H
#define C_COMPILER_PARSER_H

#include "syntax/literal.h"
#include "syntax/statement.h"
#include "syntax/syntax_node.h"
#include "token.h"
#include <vector>

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
        return parse_return_statement();
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

    std::unique_ptr<return_statement> parse_return_statement()
    {
        const auto &current = current_token();
        if (!match(token_type::keyword) || current.text != "return")
        {
            throw std::runtime_error("Expected a 'return' keyword");
        }
        auto ret_statement = std::make_unique<return_statement>(current);
        advance();

        if (match(token_type::integer_literal))
        {
            ret_statement->set_expression(
                std::make_unique<integer_literal>(current_token())
            );
            advance();
        }

        if (!match(token_type::semicolon))
        {
            throw std::runtime_error("Expected a ';'");
        }
        advance();

        return ret_statement;
    }

    /// <summary>
    /// Matches the current token against a single `token_type`.
    /// </summary>
    /// <param name="first">The `token_type` to match.</param>
    /// <returns>`true` if matches. `false` otherwise.</returns>
    bool match(const token_type &first) const
    {
        return current_token().type == first;
    }

    template <typename... Ts>
    using are_token_types = std::conjunction<std::is_same<Ts, token_type>...>;

    /// <summary>
    /// Matches the current token against a variable number of `token_type` arguments.
    /// </summary>
    /// <param name="first">The first token to match.</param>
    /// <param name="rest">The remaining tokens to match.</param>
    /// <returns>`true` if at least one of the arguments matches. `false` otherwise.</returns>
    template <typename... Args, typename = std::enable_if_t<are_token_types<Args...>::value>>
    bool match(const token_type &first, const Args &... rest) const
    {
        return match(first) && match(rest...);
    }

private:
    std::size_t index_;
    const_reference<std::vector<token>> tokens_;
};

#endif
