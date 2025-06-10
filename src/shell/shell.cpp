#include "shell.hpp"

#include <iostream>

int shell(std::string cmd)
{
    std::cout << cmd << std::endl;
    return std::system(cmd.c_str());
}