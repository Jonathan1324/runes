#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

#include "spell.hpp"
#include "util/trim.hpp"

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

    std::vector<Spell> spells;


    Runescript runescript;


    // read lines
    std::string line;
    while (std::getline(infile, line))
    {
        line = trim(line);

        if (line.empty()) continue;

        if (line.find("spell ") == 0)
        {
            Spell spell = parseSpell(infile, trim(line.substr(6)));
            spells.push_back(spell);
            continue;
        }
        else if (line.find("#") == 0)
        {
            std::string macro = line.substr(1);
            if (macro.find("define ") == 0)
            {
                std::string definition = macro.substr(7);
                size_t equal = definition.find("=");
                std::string name = trim(definition.substr(0, equal - 1));
                std::string value = trim(definition.substr(equal + 1));

                Constant constant;
                constant.value = value;

                runescript.constants[name] = constant;
            }
            if (macro.find("include ") == 0)
            {
                std::string file = macro.substr(7);
                //TODO
            }
            continue;
        }

        std::cout << "Unhandled line \"" << line << '\"' << std::endl;
    }

    infile.close();

    runescript.spells = mapSpells(spells);

    //TODO: debug print spells
    for (size_t i = 0; i < spells.size(); i++)
    {
        Spell spell = spells[i];

        std::cout << "Spell: " << spell.name << '\n';

        for (size_t j = 0; j < spell.commands.size(); j++)
        {
            std::cout << "\tCommand: " << spell.commands[j].command << '\n';
        }

        std::cout << std::endl;
    }

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
        std::cout << "No spell specified. Usage: runes cast <spell>" << std::endl;
        return 0;
    }

    for (size_t i = 0; i < casts.size(); i++)
    {
        auto spell = seekSpell(&runescript, casts[i]);
        if (spell)
            castSpell(runescript, *spell);
        else
            std::cerr << "Couldn't find spell '" << casts[i] << "'" << std::endl;
    }

    //TODO: add variables

    return 0;
}