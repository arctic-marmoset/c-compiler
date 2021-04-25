#ifndef C_COMPILER_DEFINITIONS_H
#define C_COMPILER_DEFINITIONS_H

#include <string_view>

namespace cc::keyworddefs {

inline constexpr std::string_view char_keyword = "char";
inline constexpr std::string_view int_keyword = "int";
inline constexpr std::string_view double_keyword = "double";
inline constexpr std::string_view float_keyword = "float";
inline constexpr std::string_view struct_keyword = "struct";
inline constexpr std::string_view enum_keyword = "enum";
inline constexpr std::string_view void_keyword = "void";
inline constexpr std::string_view short_keyword = "short";
inline constexpr std::string_view long_keyword = "long";

inline constexpr std::string_view const_keyword = "const";
inline constexpr std::string_view static_keyword = "static";

inline constexpr std::string_view if_keyword = "if";
inline constexpr std::string_view else_keyword = "else";
inline constexpr std::string_view for_keyword = "for";
inline constexpr std::string_view while_keyword = "while";
inline constexpr std::string_view break_keyword = "break";
inline constexpr std::string_view continue_keyword = "continue";

inline constexpr std::string_view return_keyword = "return";

}

namespace cc::chardefs {

inline constexpr char period = '.';

inline constexpr char equal = '=';

inline constexpr char plus = '+';
inline constexpr char dash = '-';
inline constexpr char asterisk = '*';
inline constexpr char forward_slash = '/';
inline constexpr char back_slash = '\\';

inline constexpr char single_quote = '\'';
inline constexpr char double_quote = '"';

inline constexpr char pipe = '|';
inline constexpr char ampersand = '&';
inline constexpr char bang = '!';
inline constexpr char tilde = '~';

inline constexpr char open_paren = '(';
inline constexpr char close_paren = ')';
inline constexpr char open_angle = '<';
inline constexpr char close_angle = '>';
inline constexpr char open_brace = '{';
inline constexpr char close_brace = '}';
inline constexpr char open_square = '[';
inline constexpr char close_square = ']';

inline constexpr char comma = ',';
inline constexpr char semicolon = ';';

inline constexpr char apostrophe = '\'';
inline constexpr char quote = '\"';
inline constexpr char backslash = '\\';

inline constexpr char question = '?';
inline constexpr char colon = ':';

inline constexpr char underscore = '_';

inline constexpr char cr = '\r';
inline constexpr char lf = '\n';
inline constexpr char eof = '\0';

}

#endif
