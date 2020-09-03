#ifndef C_COMPILER_SYMBOL_TABLE_H
#define C_COMPILER_SYMBOL_TABLE_H

#include <any>
#include <stdexcept>
#include <unordered_map>

// TODO: table::value_type::second_type should countain symbol information

class symbol_table
{
    using table = std::unordered_map<std::string_view, bool>;

public:
    symbol_table(const symbol_table *enclosing = nullptr)
        : enclosing_(enclosing)
    {
    }

    table::value_type::second_type get(table::key_type identifier) const
    {
        if (const auto it = symbols_.find(identifier); it != symbols_.end())
        {
            return it->second;
        }
        else
        {
            if (enclosing_)
            {
                return enclosing_->get(identifier);
            }
        }

        throw std::runtime_error("Identifier '" + std::string(identifier) + "' is undefined");
    }

    bool has_declared(table::key_type identifier) const
    {
        if (has_declared_in_scope(identifier))
        {
            return true;
        }
        else
        {
            if (enclosing_)
            {
                return enclosing_->has_declared(identifier);
            }
        }

        return false;
    }

    bool has_declared_in_scope(table::key_type identifier) const
    {
        return symbols_.find(identifier) != symbols_.end();
    }

    bool has_defined(table::key_type identifier) const
    {
        if (get(identifier))
        {
            return true;
        }
        else
        {
            if (enclosing_)
            {
                return enclosing_->has_defined(identifier);
            }
        }

        return false;
    }

    void declare(table::key_type identifier)
    {
        symbols_.insert({ identifier, false });
    }

    void define(table::key_type identifier, table::value_type::second_type value)
    {
        symbols_.insert_or_assign(identifier, value);
    }

private:
    table symbols_;
    const symbol_table *enclosing_;
};

#endif
