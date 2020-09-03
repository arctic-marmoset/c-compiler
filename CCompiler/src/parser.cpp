#include "parser.h"

#include "syntax/binary_expression.h"
#include "syntax/compound_statement.h"
#include "syntax/declaration_reference_expression.h"
#include "syntax/literal.h"
#include "syntax/parenthesized_expression.h"
#include "syntax/primary_expression.h"
#include "syntax/return_statement.h"
#include "syntax/variable_declaration.h"

std::unique_ptr<primary_expression> parser::parse_literal()
{
    const auto &current = current_token();

    if (!match(token_type::integer_literal,
               token_type::double_literal,
               token_type::float_literal,
               token_type::string_literal))
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

    if (!consume(token_type::open_parenthesis))
    {
        throw std::runtime_error("Expected a '('");
    }

    auto expression = parse_expression();

    if (!consume(token_type::close_parenthesis))
    {
        throw std::runtime_error("Expected a ')'");
    }

    return std::make_unique<parenthesized_expression>(
        start_token,
        std::move(expression)
    );
}

std::unique_ptr<declaration_reference_expression> parser::parse_declaration_reference_expression()
{
    const auto &identifier = current_token();

    if (!consume(token_type::identifier))
    {
        throw std::runtime_error("Expected an lvalue");
    }

    if (!symbols_.is_declared(identifier.text))
    {
        auto message = std::string("Identifier '") + identifier.text + "' is undefined";
        throw std::runtime_error(message);
    }

    return std::make_unique<declaration_reference_expression>(identifier);
}

std::unique_ptr<primary_expression> parser::parse_primary_expression()
{
    const auto &current = current_token();

    switch (current.type)
    {
    case token_type::open_parenthesis:
        return parse_parenthesized_expression();

    case token_type::identifier:
        return parse_declaration_reference_expression();

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

    if (!consume(token_type::return_keyword))
    {
        throw std::runtime_error("Expected a 'return' keyword");
    }

    if (consume(token_type::semicolon))
    {
        return std::make_unique<return_statement>(return_token);
    }

    auto return_expression = parse_expression();

    if (!consume(token_type::semicolon))
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

    if (!consume(token_type::open_brace))
    {
        throw std::runtime_error("Expected a '{'");
    }

    auto statements = std::make_unique<compound_statement>(start);

    while (!consume(token_type::close_brace))
    {
        statements->add_statement(
            parse_statement()
        );
    }

    return statements;
}

std::unique_ptr<variable_declaration> parser::parse_variable_declaration()
{
    const auto &type_specifier = current_token();

    if (!consume(token_type::int_keyword))
    {
        throw std::runtime_error("Expected a type specifier");
    }

    const auto &identifier = current_token();

    if (!consume(token_type::identifier))
    {
        throw std::runtime_error("Expected an identifier");
    }

    symbols_.declare(identifier.text);

    if (consume(token_type::semicolon))
    {
        return std::make_unique<variable_declaration>(
            type_specifier,
            identifier
        );
    }

    if (!consume(token_type::assign))
    {
        throw std::runtime_error("Expected a '='");
    }

    auto initializer = parse_expression();

    if (!consume(token_type::semicolon))
    {
        throw std::runtime_error("Expected a ';'");
    }

    symbols_.define(identifier.text, true);

    return std::make_unique<variable_declaration>(
        type_specifier,
        identifier,
        std::move(initializer)
    );
}

std::unique_ptr<binary_expression> parser::parse_binary_expression()
{
    // TODO: Binary expressions should not all be right-associative

    auto left = parse_primary_expression();

    const auto &op = current_token();
    if (!consume(token_type::plus))
    {
        throw std::runtime_error("Expected a binary operator");
    }

    auto right = parse_expression();

    return std::make_unique<binary_expression>(
        std::move(left),
        op,
        std::move(right)
    );
}

std::unique_ptr<expression> parser::parse_expression()
{
    const auto &next = peek_token(1);

    if (next.type == token_type::plus)
    {
        return parse_binary_expression();
    }
    else
    {
        return parse_primary_expression();
    }
}

std::unique_ptr<statement> parser::parse_statement()
{
    const auto &current = current_token();

    switch (current.type)
    {
    case token_type::return_keyword:
        return parse_return_statement();
    case token_type::int_keyword:
        return parse_variable_declaration();
    case token_type::open_brace:
        return parse_compound_statement();
    default:
        return parse_expression();
    }
}
