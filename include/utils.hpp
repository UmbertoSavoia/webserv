#ifndef UTILS_HPP
#define UTILS_HPP

#include <unistd.h>
#include <fcntl.h>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include "Config.hpp"
#include "Server.hpp"

bool								ft_isspace(char c);
std::pair<std::string, bool>		iskeywords(std::string src, std::string keywords[], int num);
std::pair<std::string, std::string>	split_param(std::string src);

#endif
