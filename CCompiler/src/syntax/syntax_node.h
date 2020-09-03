#ifndef C_COMPILER_SYNTAX_NODE_H
#define C_COMPILER_SYNTAX_NODE_H

#include "syntax/syntax_type.h"
#include "token.h"
#include <vector>

class syntax_node
{
protected:
    template <typename T>
    using const_reference = std::reference_wrapper<const T>;

public:
    virtual ~syntax_node() = default;

    syntax_node(const syntax_node &) = delete;
    syntax_node(syntax_node &&) = delete;
    syntax_node &operator=(const syntax_node &) = delete;
    syntax_node &operator=(syntax_node &&) = delete;

    /**
     * @brief  Returns the type of this node.
     * @return The type of this node.
     */
    virtual syntax_type type() const = 0;

    /**
     * @brief  Constructs an std::string that describes this node.
     * @return A description of this node.
     */
    virtual std::string to_string() const = 0;

    source_position source_position() const
    {
        return trigger_token().pos;
    }

    const std::vector<syntax_node *> &children() const
    {
        return children_;
    }

    const token &trigger_token() const
    {
        return trigger_token_;
    }

    /**
     * @brief Constructs an std::string that depicts this node and all its children in the
     *        form of a pretty-printed tree.
     *
     * @param[in] indent A string that determines what the indent for this node looks like.
     *
     * @param[in] last   Whether this node should be considered to be the last out of its
     *                   siblings. Affects the visual representation of the branch leading
     *                   to this node.
     *
     * @param[in] root   Whether this node should be considered to be the root node. Affects
     *                   whether this node will be prefixed by a visual representation of a
     *                   branch.
     *
     * @return           A visual representation of the syntax tree rooted at this node.
     */
    virtual std::string tree_representation(std::string indent = std::string(),
                                            bool last = true, 
                                            bool root = true) const
    {
        std::ostringstream ss;

        // Indent this node with the indent string
        ss << indent;

        // Only show a branch if this is not the root node
        if (!root)
        {
            // If last out of siblings, show a terminated branch
            if (last)
            {
                ss << "`-";
                indent += "  ";
            }
            // Otherwise, show a three-way junction
            else
            {
                ss << "|-";
                indent += "| ";
            }
        }

        // Add this node's description
        ss << to_string();

        // Add each child tree
        for (std::size_t i = 0; i < children_.size(); i++)
        {
            ss << '\n';
            ss << children_[i]->tree_representation(indent, i == children_.size() - 1, false);
        }

        return ss.str();
    }

protected:
    syntax_node(const token &trigger_token)
        : trigger_token_(trigger_token)
    {
    }

protected:
    std::vector<syntax_node *> children_;
    const_reference<token> trigger_token_;
};

#endif
