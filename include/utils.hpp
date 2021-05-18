#ifndef UTILS_HPP
#define UTILS_HPP

#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <errno.h>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <signal.h>
#include <algorithm>

#include <exception>
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "Config.hpp"
#include "Server.hpp"
#include "Locations.hpp"
#include "Handler.hpp"
#include "Request.hpp"
#include "Client.hpp"
#include "Response.hpp"

#define PRINT(x) std::cout << x << std::endl;

bool								ft_isspace(char c);
std::pair<std::string, bool>		iskeywords(std::string src, std::string keywords[], int num);
std::pair<std::string, std::string>	split_param(std::string src);
std::string							getPath(std::string src, int pos);
int									get_max_fd(std::vector<Server>* server);
void								log(std::string msg);

#endif
