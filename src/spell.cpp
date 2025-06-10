#include "spell.hpp"

#include "util/trim.hpp"
#include "util/string.hpp"
#include "shell/shell.hpp"

#include <iostream>

Spell parseSpell(std::ifstream& file, const std::string& header)
{
    Spell spell;
    if (header.find("\"") == 0)
        spell.name = parseString(header);
    else
    {
        std::string name = "";
        for (size_t i = 0; i < header.size(); i++)
        {
            if (!isalnum(header[i]))
                break;
            name += header[i];
        }
        spell.name = name;
    }

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
    for (size_t i = 0; i < spell.commands.size(); i++)
    {
        Command command = spell.commands[i];
        if (command.command.find("!") == 0)
        {
            std::string trimmed = trim(command.command.substr(1));
            std::string cmd;
            for (size_t j = 0; j < trimmed.size(); j++)
            {
                if (trimmed[j] == '$')
                {
                    j++;
                    if (trimmed[j] == '$')
                    {
                        cmd += trimmed;
                        continue;
                    }
                    else if (trimmed[j] == '{')
                    {
                        j++;
                        std::string identifier;
                        while (trimmed[j] != '}' || j >= trimmed.size())
                        {
                            identifier += trimmed[j];
                            j++;
                        }
                        j++;
                        auto it = spell.variables.find(identifier);
                        if (it != spell.variables.end())
                            cmd += it->second.value;
                        else
                            std::cerr << "Unknown variable: " << identifier << std::endl;
                    }
                }
                cmd += trimmed[j];
            }

            std::cout << cmd << std::endl;
            int ret = shell(cmd);
            (void)ret;
        }
        else if (command.command.find("$") == 0)
        {
            std::string trimmed = trim(command.command.substr(1));
            std::string cmd;
            for (size_t j = 0; j < trimmed.size(); j++)
            {
                if (trimmed[j] == '$')
                {
                    j++;
                    if (trimmed[j] == '$')
                    {
                        cmd += trimmed;
                        continue;
                    }
                    else if (trimmed[j] == '{')
                    {
                        j++;
                        std::string identifier;
                        while (trimmed[j] != '}' || j >= trimmed.size())
                        {
                            identifier += trimmed[j];
                            j++;
                        }
                        j++;
                        auto it = spell.variables.find(identifier);
                        if (it != spell.variables.end())
                            cmd += it->second.value;
                        else
                            std::cerr << "Unknown variable: " << identifier << std::endl;
                    }
                }
                cmd += trimmed[j];
            }

            int ret = shell(cmd);
            (void)ret;
        }
        else if (command.command.find("cast ") == 0)
        {
            std::string args = trim(command.command.substr(5));
            std::string cast;
            
            if (args.find("\"") == 0)
                cast = parseString(args);
            else
            {
                std::string name = "";
                for (size_t i = 0; i < args.size(); i++)
                {
                    if (!isalnum(args[i]))
                        break;
                    name += args[i];
                }
                cast = name;
            }

            std::string arguments = trim(args.substr(cast.size()));

            auto spell = seekSpell(&runescript, cast);
            if (!spell)
            {
                std::cerr << "Couldn't find spell '" << cast << "'" << std::endl;
            }
            else
            {
                if (!arguments.empty())
                {
                    for (size_t j = 0; j < arguments.size(); j++)
                    {
                        size_t next = arguments.find(",", j);
                        std::string argument;
                        if (next == std::string::npos)
                        {
                            argument = trim(arguments.substr(j));
                            j = arguments.size() - 1;
                        }
                        else
                        {
                            argument = trim(arguments.substr(j, next - 1));
                            j = next;
                        }

                        size_t equal = argument.find("=");
                        if (equal == std::string::npos)
                        {
                            continue;
                        }
                        
                        std::string name = trim(argument.substr(0, equal));
                        std::string value = trim(argument.substr(equal + 1));

                        Variable var;
                        var.value = value;

                        (*spell).variables[name] = var;
                    }
                }
                castSpell(runescript, *spell);
            }
        }

        else if (command.command.find("=") != std::string::npos)
        {
            size_t equal = command.command.find("=");
            std::string name = trim(command.command.substr(0, equal - 1));
            std::string value = trim(command.command.substr(equal + 1));

            Variable var;
            var.value = value;

            spell.variables[name] = var;
        }

        else
        {
            std::cerr << "Unknown command '" << command.command
                << "' in spell " << spell.name << std::endl;
        }
    }
    spell.variables.clear();
}