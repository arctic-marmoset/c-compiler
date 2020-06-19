#ifndef C_COMPILER_DEFINITIONS_H
#define C_COMPILER_DEFINITIONS_H

namespace keyworddefs
{
    inline constexpr const char *char_kw   = "char";
    inline constexpr const char *int_kw    = "int";
    inline constexpr const char *double_kw = "double";
    inline constexpr const char *float_kw  = "float";
    inline constexpr const char *struct_kw = "struct";
    inline constexpr const char *enum_kw   = "enum";
    inline constexpr const char *void_kw   = "void";
    inline constexpr const char *short_kw  = "short";
    inline constexpr const char *long_kw   = "long";

    inline constexpr const char *const_kw  = "const";
    inline constexpr const char *static_kw = "static";

    inline constexpr const char *if_kw       = "if";
    inline constexpr const char *else_kw     = "else";
    inline constexpr const char *for_kw      = "for";
    inline constexpr const char *while_kw    = "while";
    inline constexpr const char *break_kw    = "break";
    inline constexpr const char *continue_kw = "continue";

    inline constexpr const char *return_kw = "return";
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
