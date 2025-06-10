#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

#include "spell.hpp"
#include "util/trim.hpp"
#include "util/string.hpp"

namespace fs = std::filesystem;

int main(int argc, const char* argv[])
{
    std::string filename_to_find = "Runescript";

    fs::path found_path;
    bool found = false;

    // Search through directory
    for (const auto& entry : fs::directory_iterator(fs::current_path()))
    {
        if (entry.is_regular_file() && entry.path().filename() == filename_to_find)
        {
            found_path = entry.path();
            found = true;
            break;
        }
    }

    if (!found)
    {
        std::cerr << "Didn't find \"" << filename_to_find << "\".\n";
        return 1;
    }

    // open file
    std::ifstream infile(found_path);
    if (!infile)
    {
        std::cerr << "Couldn't open " << found_path << std::endl;
        return 1;
    }

    Runescript runescript = parseRunescript(infile);

    infile.close();

    //TODO: I don't like this way
    std::vector<std::string> casts;

    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];
        if (arg.compare("cast") == 0)
        {
            if (i + 1 >= argc)
            {
                std::cout << "Usage: runes cast <spell>" << std::endl;
                return 1;
            }
            casts.push_back(argv[++i]);
        }
    }

    if (casts.empty())
    {
        casts.push_back("all");
    }

    for (size_t i = 0; i < casts.size(); i++)
    {
        auto spell = seekSpell(runescript.spells, casts[i]);
        if (spell)
            castSpell(runescript.spells, runescript.constants, *spell);
        else
            std::cerr << "Couldn't find spell '" << casts[i] << "'" << std::endl;
    }

    //TODO: add variables

    return 0;
}