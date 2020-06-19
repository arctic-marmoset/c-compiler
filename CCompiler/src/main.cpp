#include "lexer.h"

#include <algorithm>
#include <fstream>

int main(int argc, char **argv)
{
#ifdef NDEBUG
    std::string file = argv[1];
    std::ifstream fs(file);
#endif

    while (true)
    {
        std::string line;

    #ifdef NDEBUG
        std::getline(fs, line);
    #else
        std::getline(std::cin, line);
    #endif

        if (std::all_of(line.begin(), line.end(), isspace))
        {
            break;
        }

        lexer lex = lexer(line);
        auto tokens = lex.lex_contents();
        for (const auto &token : tokens)
        {
            std::cout << token.type << ": " << token.text << "\n";
        }
    }
    return 0;
}
