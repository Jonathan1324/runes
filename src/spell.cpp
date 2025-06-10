#include "spell.hpp"

#include "util/trim.hpp"
#include "util/string.hpp"
#include "shell/shell.hpp"

#include <iostream>

Spell parseSpell(std::ifstream& file, const std::string& header)
{
    Spell spell;
    spell.name = parseString(header);

    std::string line;
    while (std::getline(file, line))
    {
        line = trim(line);

        if (line.empty()) continue;
        if (line == "{") continue;
        if (line == "}") break;


        Command command;
        command.command = line;

        spell.commands.push_back(command);
    }

    return spell;
}


std::unordered_map<std::string, Spell> mapSpells(const std::vector<Spell>& spells)
{
    std::unordered_map<std::string, Spell> map;
    for (size_t i = 0; i < spells.size(); i++)
    {
        map[spells[i].name] = spells[i];
    }
    return map;
}

std::optional<Spell> seekSpell(Runescript *runescript, const std::string& name)
{
    auto it = runescript->spells.find(name);
    if (it != runescript->spells.end())
        return it->second;
    return std::nullopt;
}

void castSpell(Runescript runescript, Spell spell)
{
    for (size_t j = 0; j < spell.commands.size(); j++)
    {
        Command command = spell.commands[j];
        if (command.command.find("!") == 0)
        {
            std::string cmd = trim(command.command.substr(1));
            int ret = shell(cmd);
            (void)ret;
        }
        else if (command.command.find("cast ") == 0)
        {
            std::string cast = trim(command.command.substr(5));
            auto spell = seekSpell(&runescript, cast);
            if (spell)
                castSpell(runescript, *spell);
            else
                std::cerr << "Couldn't find spell '" << cast << "'" << std::endl;
        }

        else
        {
            std::cerr << "Unknown command '" << command.command
                << "' in spell " << spell.name << std::endl;
        }
    }
}