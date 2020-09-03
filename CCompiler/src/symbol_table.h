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
    table::value_type::second_type get(table::key_type identifier)
    {
        const auto it = symbols_.find(identifier);
        return it->second;
    }

    bool is_declared(table::key_type identifier)
    {
        return symbols_.find(identifier) != symbols_.end();
    }

    bool is_defined(table::key_type identifier)
    {
        return get(identifier);
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
};

#endif
