#ifndef C_COMPILER_DEFINITIONS_H
#define C_COMPILER_DEFINITIONS_H

#include <string_view>

namespace keyworddefs
{
    inline constexpr std::string_view char_kw   = "char";
    inline constexpr std::string_view int_kw    = "int";
    inline constexpr std::string_view double_kw = "double";
    inline constexpr std::string_view float_kw  = "float";
    inline constexpr std::string_view struct_kw = "struct";
    inline constexpr std::string_view enum_kw   = "enum";
    inline constexpr std::string_view void_kw   = "void";
    inline constexpr std::string_view short_kw  = "short";
    inline constexpr std::string_view long_kw   = "long";

    inline constexpr std::string_view const_kw  = "const";
    inline constexpr std::string_view static_kw = "static";

    inline constexpr std::string_view if_kw       = "if";
    inline constexpr std::string_view else_kw     = "else";
    inline constexpr std::string_view for_kw      = "for";
    inline constexpr std::string_view while_kw    = "while";
    inline constexpr std::string_view break_kw    = "break";
    inline constexpr std::string_view continue_kw = "continue";

    inline constexpr std::string_view return_kw = "return";
}

namespace chardefs
{
    inline constexpr char period = '.';

    inline constexpr char equal = '=';

    inline constexpr char plus          = '+';
    inline constexpr char dash          = '-';
    inline constexpr char asterisk      = '*';
    inline constexpr char forward_slash = '/';

    inline constexpr char pipe      = '|';
    inline constexpr char ampersand = '&';
    inline constexpr char bang      = '!';
    inline constexpr char tilde     = '~';

    inline constexpr char open_paren   = '(';
    inline constexpr char close_paren  = ')';
    inline constexpr char open_angle   = '<';
    inline constexpr char close_angle  = '>';
    inline constexpr char open_brace   = '{';
    inline constexpr char close_brace  = '}';
    inline constexpr char open_square  = '[';
    inline constexpr char close_square = ']';

    inline constexpr char comma     = ',';
    inline constexpr char semicolon = ';';

    inline constexpr char apostrophe = '\'';
    inline constexpr char quote      = '\"';
    inline constexpr char back_slash = '\\';

    inline constexpr char question = '?';
    inline constexpr char colon    = ':';

    inline constexpr char underscore = '_';

    inline constexpr char eof = '\0';
}

#endif // !C_COMPILER_DEFINITIONS_H
