#ifndef C_COMPILER_LITERAL_H
#define C_COMPILER_LITERAL_H

#include "syntax/syntax_node.h"

class integer_literal : public syntax_node
{
public:
    integer_literal(const token &underlying_token)
        : syntax_node(underlying_token)
    {
    }

    syntax_type type() const override
    {
        return syntax_type::integer_literal;
    }

    std::string to_string() const override
    {
        std::ostringstream ss;

        const auto &[_, text, pos] = trigger_token_.get();

        ss << "integer_literal"         " "
            + pos.to_string("<", ">") + " "
            "'int'" " " + text;

        return ss.str();
    }
};

#endif
