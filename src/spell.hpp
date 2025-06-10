#pragma once

#include <string>
#include <vector>
#include <optional>
#include <fstream>
#include <unordered_map>
#include <optional>

struct Constant {
    std::string value;
};


struct Command {
    std::string command;
};

struct Variable {
    std::string value;
};

struct Spell {
    std::string name;
    std::vector<Command> commands;
    std::unordered_map<std::string, Variable> variables;
};

#include "runescript.hpp"

Spell parseSpell(std::ifstream& file, const std::string& header);
std::unordered_map<std::string, Spell> mapSpells(const std::vector<Spell>& spells);
std::optional<Spell> seekSpell(std::unordered_map<std::string, Spell> spells, const std::string& name);
void castSpell(std::unordered_map<std::string, Spell> spells, std::unordered_map<std::string, Constant> constants, Spell spell);