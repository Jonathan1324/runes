#pragma once

#include <unordered_map>
#include "spell.hpp"

struct Constant {
    std::string value;
};

struct Runescript {
    std::unordered_map<std::string, Constant> constants;
    std::unordered_map<std::string, Spell> spells;
};