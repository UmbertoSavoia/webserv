#ifndef UTILS_HPP
#define UTILS_HPP

#include <unistd.h>
#include <fcntl.h>
#include <exception>
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "Config.hpp"
#include "Server.hpp"
#include "Locations.hpp"

#define PRINT(x) std::cout << x << std::endl;

bool								ft_isspace(char c);
std::pair<std::string, bool>		iskeywords(std::string src, std::string keywords[], int num);
std::pair<std::string, std::string>	split_param(std::string src);
std::string							getPath(std::string src, int pos);

#endif
