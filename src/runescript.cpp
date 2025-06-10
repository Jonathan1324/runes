#include "runescript.hpp"

#include <vector>
#include <iostream>
#include <optional>

#include "util/trim.hpp"
#include "util/string.hpp"

Runescript parseRunescript(std::ifstream& file)
{
    Runescript runescript;

    std::vector<Spell> spells;

    // read lines
    std::string line;
    while (std::getline(file, line))
    {
        line = trim(line);

        if (line.empty()) continue;

        if (line.find("spell ") == 0)
        {
            Spell spell = parseSpell(file, trim(line.substr(6)));
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
                std::string path = parseString(macro.substr(7));
                
                // open file
                std::ifstream file(path);
                if (!file)
                {
                    std::cerr << "Couldn't open " << path << std::endl;
                    //TODO
                }

                Runescript includeRunescript = parseRunescript(file);

                for (const auto& constant : includeRunescript.constants)
                    runescript.constants[constant.first] = constant.second;

                for (const auto& spell : includeRunescript.spells)
                {
                    runescript.spells[spell.first] = spell.second;
                }

                file.close();
            }
            continue;
        }

        std::cerr << "Unhandled line \"" << line << '\"' << std::endl;
    }

    std::unordered_map<std::string, Spell> newSpells = mapSpells(spells);
    for (const auto& spell : newSpells)
        runescript.spells[spell.first] = spell.second;

    return runescript;
}