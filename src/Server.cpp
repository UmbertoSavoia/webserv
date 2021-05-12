#include "../include/utils.hpp"

Server::Server(void) :
	socket_fd(-1), port(-1), params(), locations()
{
}

Server::~Server(void) {}

std::map<std::string, std::string>& Server::getParams(void)
{
	return params;
}

std::vector<Locations>&				Server::getLocations(void)
{
	return locations;
}