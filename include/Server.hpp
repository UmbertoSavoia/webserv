#ifndef SERVER_HPP
#define SERVER_HPP

#include "utils.hpp"
class Locations;

class Server
{
	private:
		int									socket_fd;
		int									port;
		bool								autoidx;
		std::map<std::string, std::string>	params;
		std::vector<Locations>				locations;

	public:
		Server(void) : socket_fd(-1), port(-1), autoidx(false), params(), locations() {}
		~Server(void) {}
		bool&								getautoidx(void) { return autoidx; }
		std::map<std::string, std::string>&	getParams(void) { return params; }
		std::vector<Locations>&				getLocations(void) { return locations; }
};

#endif
