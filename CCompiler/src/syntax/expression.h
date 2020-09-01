#ifndef C_COMPILER_EXPRESSION_H
#define C_COMPILER_EXPRESSION_H

#include "syntax/syntax_node.h"

class expression : public syntax_node
{
public:
    ~expression() = default;

    expression(const expression &) = delete;
    expression(expression &&) = delete;
    expression &operator=(const expression &) = delete;
    expression &operator=(expression &&) = delete;

protected:
    expression(const token &trigger_token)
        : syntax_node(trigger_token)
    {
    }
};

class primary_expression : public expression
{
public:
    ~primary_expression() = default;

    primary_expression(const primary_expression &) = delete;
    primary_expression(primary_expression &&) = delete;
    primary_expression &operator=(const primary_expression &) = delete;
    primary_expression &operator=(primary_expression &&) = delete;

protected:
    primary_expression(const token &trigger_token)
        : expression(trigger_token)
    {
    }
};

// Not sure if this is the best way to avoid code duplication 
// across arithmetic types, but it works

#define DECLARE_LITERAL_SYNTAX_NODE(name, display_name)      \
    class name : public primary_expression                   \
    {                                                        \
    public:                                                  \
        explicit name(const token &trigger_token)            \
            : primary_expression(trigger_token)              \
        {                                                    \
        }                                                    \
                                                             \
        syntax_type type() const override                    \
        {                                                    \
            return syntax_type::name;                        \
        }                                                    \
                                                             \
        std::string to_string() const override               \
        {                                                    \
            const auto &[_, text, pos] = trigger_token();    \
                                                             \
            return #name                       " "           \
                   + pos.to_string("<", ">") + " "           \
                   "'" display_name "'" " " + text;          \
        }                                                    \
    }                                                        \

DECLARE_LITERAL_SYNTAX_NODE(integer_literal, "int");
DECLARE_LITERAL_SYNTAX_NODE(double_literal, "double");
DECLARE_LITERAL_SYNTAX_NODE(float_literal, "float");

// Special case for string_literal

class string_literal : public primary_expression
{
public:
    explicit string_literal(const token &trigger_token)
        : primary_expression(trigger_token)
    {
    }

    syntax_type type() const override
    {
        return syntax_type::string_literal;
    }

    std::string to_string() const override
    {
        std::ostringstream ss;

        const auto &[_, text, pos] = trigger_token();

        ss << "string_literal"              " "
              + pos.to_string("<", ">")   + " "
              "'char [" << text.size() << "]'"
              " " + text;

        return ss.str();
    }
};

class parenthesized_expression : public primary_expression
{
public:
    parenthesized_expression(const token &trigger_token,
                             std::unique_ptr<expression> enclosed_expression)
        : primary_expression(trigger_token)
        , enclosed_expression_(std::move(enclosed_expression))
    {
        children_.push_back(enclosed_expression_.get());
    }

    syntax_type type() const override
    {
        return syntax_type::parenthesized_expression;
    }

    std::string to_string() const override
    {
        const auto &pos = trigger_token().pos;

        return "parenthesized_expression"  " "
               + pos.to_string("<", ">");
    }

private:
    std::unique_ptr<expression> enclosed_expression_;
};

class binary_expression : public expression
{
public:
    binary_expression(std::unique_ptr<expression> left, 
                      const token &op, 
                      std::unique_ptr<expression> right)
        : expression(left->trigger_token())
        , left_(std::move(left))
        , right_(std::move(right))
        , operator_(op)
    {
        children_.push_back(left_.get());
        children_.push_back(right_.get());
    }

    syntax_type type() const override
    {
        return syntax_type::binary_expression;
    }

    std::string to_string() const override
    {
        const auto &pos = trigger_token().pos;

        return "binary_expression"          " "
               + pos.to_string("<", ">")  + " "
               "'" + operator_.get().text + "'";
    }

private:
    std::unique_ptr<expression> left_;
    std::unique_ptr<expression> right_;
    const_reference<token> operator_;
};

#endif
