#include "parser.h"

#include "symbol_table.h"
#include "token.h"
#include "token_type.h"
#include "syntax/binary_expression.h"
#include "syntax/compound_statement.h"
#include "syntax/declaration.h"
#include "syntax/declaration_reference_expression.h"
#include "syntax/expression.h"
#include "syntax/function_declaration.h"
#include "syntax/literal.h"
#include "syntax/parenthesized_expression.h"
#include "syntax/primary_expression.h"
#include "syntax/return_statement.h"
#include "syntax/statement.h"
#include "syntax/translation_unit_declaration.h"
#include "syntax/variable_declaration.h"

std::unique_ptr<cc::primary_expression> cc::parser::parse_literal()
{
    const auto &current = current_token();

    if (!match(cc::token_type::char_literal,
               cc::token_type::integer_literal,
               cc::token_type::double_literal,
               cc::token_type::float_literal,
               cc::token_type::string_literal))
    {
        throw std::runtime_error("Expected a literal");
    }
    advance();

    switch (current.type)
    {
    case cc::token_type::char_literal:
        return std::make_unique<cc::char_literal>(current);
    case cc::token_type::integer_literal:
        return std::make_unique<cc::integer_literal>(current);
    case cc::token_type::double_literal:
        return std::make_unique<cc::double_literal>(current);
    case cc::token_type::float_literal:
        return std::make_unique<cc::float_literal>(current);
    case cc::token_type::string_literal:
        return std::make_unique<cc::string_literal>(current);
    default:
        throw std::runtime_error("Hit unreachable branch in parse_literal()");
    }
}

std::unique_ptr<cc::parenthesized_expression> cc::parser::parse_parenthesized_expression()
{
    const auto &start_token = current_token();

    if (!consume(cc::token_type::open_parenthesis))
    {
        throw std::runtime_error("Expected a '('");
    }

    auto expr = parse_expression();

    if (!consume(cc::token_type::close_parenthesis))
    {
        throw std::runtime_error("Expected a ')'");
    }

    return std::make_unique<cc::parenthesized_expression>(start_token, std::move(expr));
}

std::unique_ptr<cc::declaration_reference_expression> cc::parser::parse_declaration_reference_expression()
{
    const auto &identifier = current_token();

    if (!consume(cc::token_type::identifier))
    {
        throw std::runtime_error("Expected an lvalue");
    }

    if (!scope_.top()->is_declared(identifier.text))
    {
        throw std::runtime_error("Identifier '" + identifier.text + "' is undefined");
    }

    return std::make_unique<cc::declaration_reference_expression>(identifier);
}

std::unique_ptr<cc::primary_expression> cc::parser::parse_primary_expression()
{
    const auto &current = current_token();

    switch (current.type)
    {
    case cc::token_type::open_parenthesis:
        return parse_parenthesized_expression();

    case cc::token_type::identifier:
        return parse_declaration_reference_expression();

    case cc::token_type::integer_literal:
    case cc::token_type::double_literal:
    case cc::token_type::float_literal:
    case cc::token_type::string_literal:
    case cc::token_type::char_literal:
        return parse_literal();

    default:
        break;
    }

    throw std::runtime_error("Expected a primary expression");
}

std::unique_ptr<cc::return_statement> cc::parser::parse_return_statement()
{
    const auto &return_token = current_token();

    if (!consume(cc::token_type::return_keyword))
    {
        throw std::runtime_error("Expected a 'return' keyword");
    }

    if (consume(cc::token_type::semicolon))
    {
        return std::make_unique<cc::return_statement>(return_token);
    }

    auto return_expression = parse_expression();

    if (!consume(cc::token_type::semicolon))
    {
        throw std::runtime_error("Expected a ';'");
    }

    return std::make_unique<cc::return_statement>(return_token, std::move(return_expression));
}

std::unique_ptr<cc::compound_statement> cc::parser::parse_compound_statement()
{
    auto local_scope = symbol_table(scope_.top());
    scope_.push(&local_scope);

    const auto &start = current_token();

    if (!consume(cc::token_type::open_brace))
    {
        throw std::runtime_error("Expected a ';' or a '{'");
    }

    auto statements = std::make_unique<cc::compound_statement>(start);

    bool has_return_statement = false;

    while (!consume(cc::token_type::close_brace))
    {
        auto stmt = parse_statement();

        if (stmt->type() == cc::syntax_type::return_statement)
        {
            has_return_statement = true;
        }

        statements->add_statement(std::move(stmt));
    }

    statements->has_return(has_return_statement);
    scope_.pop();

    return statements;
}

std::unique_ptr<cc::variable_declaration> cc::parser::parse_variable_declaration(const cc::token &type_specifier,
                                                                                 const cc::token &identifier)
{
    if (scope_.top()->is_declared_in_scope(identifier.text))
    {
        throw std::runtime_error("Redefinition of " + identifier.text + "\nFirst declaration at ");
    }

    scope_.top()->declare(identifier.text);

    if (consume(cc::token_type::semicolon))
    {
        return std::make_unique<cc::variable_declaration>(type_specifier, identifier);
    }

    if (!consume(cc::token_type::assign))
    {
        throw std::runtime_error("Expected a ';' or a '='");
    }

    auto initializer = parse_expression();

    if (!consume(cc::token_type::semicolon))
    {
        throw std::runtime_error("Expected a ';'");
    }

    scope_.top()->define(identifier.text, true);

    return std::make_unique<cc::variable_declaration>(
        type_specifier,
        identifier,
        std::move(initializer)
    );
}

std::unique_ptr<cc::function_declaration> cc::parser::parse_function_declaration(const cc::token &type_specifier,
                                                                                 const cc::token &identifier)
{
    if (!consume(cc::token_type::open_parenthesis))
    {
        throw std::runtime_error("Expected a '('");
    }

    // TODO: Parse parameter declarations

    if (!consume(cc::token_type::close_parenthesis))
    {
        throw std::runtime_error("Expected a ')'");
    }

    bool is_redeclared = scope_.top()->is_declared(identifier.text);

    if (!is_redeclared)
    {
        scope_.top()->declare(identifier.text);
    }

    if (consume(cc::token_type::semicolon))
    {
        return std::make_unique<cc::function_declaration>(
            type_specifier,
            identifier,
            nullptr,
            is_redeclared
        );
    }

    if (scope_.top()->is_defined(identifier.text))
    {
        throw std::runtime_error("Redefinition of " + identifier.text);
    }

    auto definition = parse_compound_statement();

    if (type_specifier.type != cc::token_type::void_keyword && !definition->returns())
    {
        throw std::runtime_error("Not all control paths return a value");
    }

    scope_.top()->define(identifier.text, true);

    return std::make_unique<cc::function_declaration>(
        type_specifier,
        identifier,
        std::move(definition),
        is_redeclared
    );
}

std::unique_ptr<cc::binary_expression> cc::parser::parse_binary_expression()
{
    // TODO: Binary expressions should not all be right-associative

    auto left = parse_primary_expression();

    const auto &op = current_token();

    if (!consume(cc::token_type::plus) && !consume(cc::token_type::assign))
    {
        throw std::runtime_error("Expected a binary operator");
    }

    auto right = parse_expression();

    return std::make_unique<cc::binary_expression>(op, std::move(left), std::move(right));
}

std::unique_ptr<cc::expression> cc::parser::parse_expression()
{
    const auto &next = peek_token(1);

    if (next.type == cc::token_type::plus || next.type == cc::token_type::assign)
    {
        return parse_binary_expression();
    }

    return parse_primary_expression();
}

std::unique_ptr<cc::statement> cc::parser::parse_expression_statement()
{
    auto expr = parse_expression();
    if (!consume(cc::token_type::semicolon))
    {
        throw std::runtime_error("Expected a ';'");
    }
    return expr;
}

std::unique_ptr<cc::statement> cc::parser::parse_statement()
{
    const auto &current = current_token();

    switch (current.type)
    {
    case cc::token_type::return_keyword:
        return parse_return_statement();
    case cc::token_type::int_keyword:
        return parse_declaration();
    case cc::token_type::open_brace:
        return parse_compound_statement();
    default:
        return parse_expression_statement();
    }
}

std::unique_ptr<cc::declaration> cc::parser::parse_declaration()
{
    const auto &type_specifier = current_token();

    if (!consume(cc::token_type::int_keyword))
    {
        throw std::runtime_error("Expected a type specifier");
    }

    const auto &identifier = current_token();

    if (!consume(cc::token_type::identifier))
    {
        throw std::runtime_error("Expected an identifier");
    }

    if (match(cc::token_type::open_parenthesis))
    {
        return parse_function_declaration(type_specifier, identifier);
    }

    return parse_variable_declaration(type_specifier, identifier);
}

std::unique_ptr<cc::translation_unit_declaration> cc::parser::parse_translation_unit()
{
    const auto &first = current_token();
    std::vector<std::unique_ptr<cc::declaration>> declarations;

    while (!match(cc::token_type::eof))
    {
        declarations.emplace_back(parse_declaration());
    }

    return std::make_unique<cc::translation_unit_declaration>(first, std::move(declarations));
}
