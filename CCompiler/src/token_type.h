#ifndef C_COMPILER_TOKEN_TYPE_H
#define C_COMPILER_TOKEN_TYPE_H

#include <iostream>
#include <type_traits>

enum class token_type
{
    integer = 0,
    double_precision,
    single_precision,

    string,
    keyword,
    modifier,
    identifier,
    
    assign,
    
    plus,
    minus,
    asterisk,
    forward_slash,
    mod,

    plus_assign,
    minus_assign,
    times_assign,
    divide_assign,
    mod_assign,

    increment,
    decrement,
    
    equals,
    not_equals,
    greater_than,
    less_than,
    
    bit_or,
    bit_and,
    bit_not,
    bit_xor,
    left_shift,
    right_shift,

    logical_or,
    logical_and,
    logical_not,

    open_paren,
    close_paren,
    open_brace,
    close_brace,
    open_angle,
    close_angle,
    open_square,
    close_square,
    comma,
    semicolon,

    eof,

    invalid
};

inline std::ostream &operator<<(std::ostream &os, token_type type)
{
    return os << static_cast<std::underlying_type_t<token_type>>(type);
}

#endif // !C_COMPILER_TOKEN_TYPE_H
