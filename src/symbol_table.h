#ifndef C_COMPILER_SYMBOL_TABLE_H
#define C_COMPILER_SYMBOL_TABLE_H

#include "token.h"

#include <any>
#include <stdexcept>
#include <unordered_map>

// TODO: table_type::value_type::second_type should contain symbol information

namespace cc {

class symbol_table
{
    using table_type = std::unordered_map<std::string_view, bool>;

public:
    explicit symbol_table(const symbol_table *enclosing = nullptr)
        : enclosing_(enclosing)
    {
    }

    table_type::value_type::second_type get(table_type::key_type identifier) const
    {
        if (const auto it = symbols_.find(identifier); it != symbols_.end())
        {
            return it->second;
        }

        if (enclosing_)
        {
            return enclosing_->get(identifier);
        }

        throw std::runtime_error("Identifier '" + std::string(identifier) + "' is undefined");
    }

    bool is_declared(table_type::key_type identifier) const
    {
        if (is_declared_in_scope(identifier))
        {
            return true;
        }

        if (enclosing_)
        {
            return enclosing_->is_declared(identifier);
        }

        return false;
    }

    bool is_declared_in_scope(table_type::key_type identifier) const
    {
        return symbols_.find(identifier) != symbols_.end();
    }

    bool is_defined(table_type::key_type identifier) const
    {
        if (get(identifier))
        {
            return true;
        }

        if (enclosing_)
        {
            return enclosing_->is_defined(identifier);
        }

        return false;
    }

    void declare(table_type::key_type identifier)
    {
        symbols_.insert({identifier, false});
    }

    void define(table_type::key_type identifier, table_type::value_type::second_type value)
    {
        symbols_.insert_or_assign(identifier, value);
    }

private:
    table_type symbols_;
    const symbol_table *enclosing_;
};

} // namespace cc

#endif
