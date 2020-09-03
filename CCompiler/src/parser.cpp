#include "parser.h"

#include "syntax/binary_expression.h"
#include "syntax/compound_statement.h"
#include "syntax/declaration_reference_expression.h"
#include "syntax/function_declaration.h"
#include "syntax/literal.h"
#include "syntax/parenthesized_expression.h"
#include "syntax/primary_expression.h"
#include "syntax/return_statement.h"
#include "syntax/translation_unit_declaration.h"
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
        throw std::runtime_error(
            "Identifier '" + identifier.text + "' is undefined"
        );
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

    bool has_return_statement = false;

    while (!consume(token_type::close_brace))
    {
        auto statement = parse_statement();

        if (statement->type() == syntax_type::return_statement)
        {
            has_return_statement = true;
        }

        statements->add_statement(
            std::move(statement)
        );
    }

    statements->set_has_return(has_return_statement);
    return statements;
}

std::unique_ptr<variable_declaration> parser::parse_variable_declaration(const token &type_specifier, 
                                                                         const token &identifier)
{
    if (symbols_.is_declared(identifier.text))
    {
        throw std::runtime_error("Redefinition of " + identifier.text);
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

std::unique_ptr<function_declaration> parser::parse_function_declaration(const token &type_specifier, 
                                                                         const token &identifier)
{
    if (!consume(token_type::open_parenthesis))
    {
        throw std::runtime_error("Expected a '('");
    }

    // TODO: Parse parameter declarations

    if (!consume(token_type::close_parenthesis))
    {
        throw std::runtime_error("Expected a ')'");
    }

    bool is_redeclared = symbols_.is_declared(identifier.text);

    if (!is_redeclared)
    {
        symbols_.declare(identifier.text);
    }

    if (consume(token_type::semicolon))
    {
        return std::make_unique<function_declaration>(
            type_specifier,
            identifier,
            nullptr,
            is_redeclared
        );
    }

    if (symbols_.is_defined(identifier.text))
    {
        throw std::runtime_error("Redefinition of " + identifier.text);
    }

    auto definition = parse_compound_statement();

    if (type_specifier.type != token_type::void_keyword && !definition->returns())
    {
        throw std::runtime_error("Not all control paths return a value");
    }

    symbols_.define(identifier.text, true);

    return std::make_unique<function_declaration>(
        type_specifier,
        identifier,
        std::move(definition),
        is_redeclared
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
        return parse_declaration();
    case token_type::open_brace:
        return parse_compound_statement();
    default:
        return parse_expression();
    }
}

std::unique_ptr<declaration> parser::parse_declaration()
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

    if (match(token_type::open_parenthesis))
    {
        return parse_function_declaration(type_specifier, identifier);
    }
    else
    {
        return parse_variable_declaration(type_specifier, identifier);
    }
}

std::unique_ptr<translation_unit_declaration> parser::parse_translation_unit()
{
    const auto &first = current_token();
    std::vector<std::unique_ptr<declaration>> declarations;

    while (!match(token_type::eof))
    {
        declarations.emplace_back(parse_declaration());
    }

    return std::make_unique<translation_unit_declaration>(
        first,
        std::move(declarations)
    );
}
