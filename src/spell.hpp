#pragma once

#include <string>
#include <vector>
#include <optional>
#include <fstream>
#include <unordered_map>
#include <optional>

struct Command {
    std::string command;
};

struct Spell {
    std::string name;
    std::vector<Command> commands;
};

struct Runescript {
    std::unordered_map<std::string, Spell> spells;
};

Spell parseSpell(std::ifstream& file, const std::string& header);
std::unordered_map<std::string, Spell> mapSpells(const std::vector<Spell>& spells);
std::optional<Spell> seekSpell(Runescript *runescript, const std::string& name);
void castSpell(Runescript runescript, Spell spell);