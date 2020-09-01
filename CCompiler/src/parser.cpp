#include "parser.h"

std::unique_ptr<primary_expression> parser::parse_literal()
{
    const auto &current = current_token();

    if (!match(
        token_type::integer_literal,
        token_type::double_literal,
        token_type::float_literal,
        token_type::string_literal
    ))
    {
        throw std::runtime_error("Expected a literal");
    }
    advance();

    switch (current.type)
    {
    case token_type::integer_literal:
        return std::make_unique<integer_literal>(current);
    case token_type::double_literal:
        return std::make_unique<double_literal>(current);
    case token_type::float_literal:
        return std::make_unique<float_literal>(current);
    case token_type::string_literal:
        return std::make_unique<string_literal>(current);
    default:
        throw std::runtime_error("Hit unreachable branch in parse_literal()");
    }
}

std::unique_ptr<parenthesized_expression> parser::parse_parenthesized_expression()
{
    const auto &start_token = current_token();

    if (!match(token_type::open_parenthesis))
    {
        throw std::runtime_error("Expected a '('");
    }
    advance();

    auto expression = parse_primary_expression();

    if (!match(token_type::close_parenthesis))
    {
        throw std::runtime_error("Expected a ')'");
    }
    advance();

    return std::make_unique<parenthesized_expression>(
        start_token,
        std::move(expression)
    );
}

std::unique_ptr<primary_expression> parser::parse_primary_expression()
{
    const auto &current = current_token();

    switch (current.type)
    {
    case token_type::open_parenthesis:
        return parse_parenthesized_expression();

    case token_type::integer_literal:
    case token_type::double_literal:
    case token_type::float_literal:
    case token_type::string_literal:
        return parse_literal();
    }

    throw std::runtime_error("Expected a primary expression");
}

std::unique_ptr<return_statement> parser::parse_return_statement()
{
    const auto &return_token = current_token();

    if (!expect(token_type::return_keyword))
    {
        throw std::runtime_error("Expected a 'return' keyword");
    }

    if (expect(token_type::semicolon))
    {
        return std::make_unique<return_statement>(return_token);
    }

    auto return_expression = parse_primary_expression();

    if (!expect(token_type::semicolon))
    {
        throw std::runtime_error("Expected a ';'");
    }

    return std::make_unique<return_statement>(
        return_token,
        std::move(return_expression)
    );
}

std::unique_ptr<compound_statement> parser::parse_compound_statement()
{
    const auto &start = current_token();

    if (!expect(token_type::open_brace))
    {
        throw std::runtime_error("Expected a '{'");
    }

    auto statements = std::make_unique<compound_statement>(start);

    while (!expect(token_type::close_brace))
    {
        statements->add_statement(
            parse_return_statement()
        );
    }

    return statements;
}
