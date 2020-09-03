#ifndef C_COMPILER_SYNTAX_TYPE_H
#define C_COMPILER_SYNTAX_TYPE_H

enum class syntax_type
{
    integer_literal = 0,
    double_literal,
    float_literal,
    string_literal,
    binary_expression,
    parenthesized_expression,
    declaration_reference_expression,
    variable_declaration,
    function_declaration,
    return_statement,
    compound_statement,
    translation_unit_declaration,
};

#endif
