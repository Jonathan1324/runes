#include "shell.hpp"

#include <iostream>

int shell(std::string cmd)
{
    return std::system(cmd.c_str());
}