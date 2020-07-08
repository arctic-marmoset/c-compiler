#include "lexer.h"
#include "parser.h"
#include <algorithm>
#include <fstream>

using namespace std::string_literals;

// TODO: Clean up interactive console vs cmdline exec selection
void run(const std::string &file_name);
void run_debug();

int main(int argc, char **argv)
{
#ifdef NDEBUG
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
        if (std::all_of(source.begin(), source.end(), std::isspace))
        {
            break;
        }

        lexer lex = lexer(source);
        auto tokens = lex.lex_contents();
        for (const auto &token : tokens)
        {
            std::cout << token.type << ": " << token.text << "\n";
        }
    }
}

void run(const std::string &file_name)
{
    auto in = std::ifstream(file_name, std::ios::in | std::ios::binary);

    if (!in)
    {
        std::cout << "Invalid filename \"" << file_name << "\"\n";
        return;
    }

    // Get char count of file. Note that istream::tellg() returns an
    // std::streampos which is not guaranteed to be the same size as
    // std::size_t (e.g. in the case that the file is greater than
    // 4 GB on a 32-bit system). We will assume that no source files
    // being read exceed this value.

    // Seek to end
    in.seekg(0, std::ios::end);

    // Get char position
    const auto count = static_cast<std::size_t>(in.tellg());

    // Construct a string with this size
    std::string source;
    source.resize(count);

    // Seek back to beginning
    in.seekg(0, std::ios::beg);

    // Read file stream into source string
    in.read(source.data(), source.size());
    in.close();

    lexer lex = lexer(source);
    auto tokens = lex.lex_contents();
    for (const auto &token : tokens)
    {
        std::cout << token.type << ": " << token.text << "\n";
    }
}
