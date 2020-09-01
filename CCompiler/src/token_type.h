#ifndef C_COMPILER_TOKEN_TYPE_H
#define C_COMPILER_TOKEN_TYPE_H

#include <iostream>
#include <type_traits>

enum class token_type
{
    integer_literal = 0,
    double_literal,
    float_literal,
    string_literal,

    char_keyword,
    int_keyword,
    double_keyword,
    float_keyword,
    struct_keyword,
    enum_keyword,
    void_keyword,
    short_keyword,
    long_keyword,
    const_keyword,
    static_keyword,
    if_keyword,
    else_keyword,
    for_keyword,
    while_keyword,
    break_keyword,
    continue_keyword,
    return_keyword,

    identifier,

    plus,
    minus,
    asterisk,
    forward_slash,
    mod,

    assign,
    plus_assign,
    minus_assign,
    times_assign,
    divide_assign,
    mod_assign,

    increment,
    decrement,

    bitwise_or,
    bitwise_and,
    bitwise_not,
    bitwise_xor,

    left_shift,
    right_shift,

    logical_or,
    logical_and,
    logical_not,

    comparison_equals,
    comparison_not_equals,
    greater_than,
    less_than,

    open_parenthesis,
    close_parenthesis,
    open_brace,
    close_brace,
    open_angle,
    close_angle,
    open_square,
    close_square,
    comma,
    semicolon,

    eof,

    unknown
};

inline std::ostream &operator<<(std::ostream &os, token_type type)
{
    return os << static_cast<std::underlying_type_t<token_type>>(type);
}

#endif // !C_COMPILER_TOKEN_TYPE_H
