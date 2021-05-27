#ifndef UTILS_HPP
#define UTILS_HPP

#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <errno.h>
#include <dirent.h>
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
#include "Headers.hpp"
#include "CGI.hpp"

#define PRINT(x) std::cout << x << std::endl;

#if __APPLE__
	#define TIMESPEC buffer.st_ctimespec.tv_sec
#elif __linux__
	#define TIMESPEC buffer.st_mtim.tv_sec
#endif

std::pair<std::string, bool>		iskeywords(std::string src, std::string keywords[], int num);
std::pair<std::string, std::string>	split_param(std::string src);
std::string							getPath(std::string src, int pos);
int									get_max_fd(std::vector<Server>* server);
void								log(std::string msg);
void								trim(std::string &str);
std::string							errorPage(std::string numCode, std::string description, std::string error_page);
std::string							autoindexGenerator(std::string path, std::string request);

#endif
