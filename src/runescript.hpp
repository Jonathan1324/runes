#pragma once

#include <unordered_map>
#include <fstream>
#include "spell.hpp"

struct Runescript {
    std::unordered_map<std::string, Constant> constants;
    std::unordered_map<std::string, Spell> spells;
};

Runescript parseRunescript(std::ifstream& file);