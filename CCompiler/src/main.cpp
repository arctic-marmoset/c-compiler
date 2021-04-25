#include "lexer.h"
#include "parser.h"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iomanip>

// TODO: Clean up interactive console vs cmdline exec selection
// TODO: Remove code duplication between run and run_debug
void run(const std::string &file_name);
void run_debug();

int main(int argc, char **argv)
{
#ifdef NDEBUG
    if (argc < 2)
    {
        std::cerr << "No input file provided\n";
        return EXIT_FAILURE;
    }
    run(argv[1]);
#else
    run_debug();
#endif
    return 0;
}

void run_debug()
{
    while (true)
    {
        std::string source;

        std::getline(std::cin, source);
        if (std::all_of(source.begin(), source.end(), [](unsigned char c) { return std::isspace(c); }))
        {
            break;
        }

        auto lexer = cc::lexer(source);
        auto tokens = lexer.lex_contents();
        std::cout << "== TOKENS ==" << "\n\n";
        for (const auto &token : tokens)
        {
            std::cout << std::left << std::setw(8)  << token.pos.to_string()
                                   << std::setw(5)  << token.type
                                   << std::setw(20) << token.text << '\n';
        }
        std::cout << '\n';

        auto par = cc::parser(tokens);

        std::shared_ptr<cc::syntax_node> root;
        try
        {
            root = par.parse_contents();
        }
        catch (const std::exception &ex)
        {
            std::cout << "Error: " << ex.what() << '\n';
            continue;
        }

        std::cout << "== AST ==" << "\n\n";
        std::cout << root->tree_representation();
        std::cout << '\n';
    }
}

void run(const std::string &file_name)
{
    auto in = std::ifstream(file_name, std::ios::binary | std::ios::ate);

    if (!in)
    {
        std::cout << "Invalid filename \"" << file_name <<"\"" "\n";
        return;
    }

    // Get char count of file. Note that istream::tellg() returns an std::streampos which is not
    // guaranteed to be the same size as std::size_t (e.g. in the case that the file is greater than
    // 4 GB on a 32-bit system). We will assume that no source files being read exceed this value.

    // Get start and end positions
    const auto end = in.tellg();
    in.seekg(0, std::ios::beg);
    const auto start = in.tellg();

    const auto size = static_cast<std::size_t>(end - start);

    // Construct a string with this size
    std::string source;
    source.resize(size);

    // Read file stream into source string
    in.read(source.data(), static_cast<std::streamsize>(size));
    in.close();

    auto lexer = cc::lexer(source);
    auto tokens = lexer.lex_contents();

    std::cout << "== TOKENS ==" << "\n\n";
    for (const auto &token : tokens)
    {
        std::cout << std::left << std::setw(8)  << token.pos.to_string()
                               << std::setw(5)  << token.type
                               << std::setw(20) << token.text << '\n';
    }
    std::cout << '\n';

    auto parser = cc::parser(tokens);

    std::unique_ptr<cc::syntax_node> root;
    try
    {
        root = parser.parse_contents();
    }
    catch (const std::exception &ex)
    {
        std::cout << "Error: " << ex.what() << '\n';
        return;
    }

    std::cout << "== AST ==" << "\n\n";
    std::cout << root->tree_representation() << '\n';
    std::cout << '\n';
}
