#ifndef SERVER_HPP
#define SERVER_HPP

#include "utils.hpp"
class Locations;

class Server
{
	private:
		int									socket_fd;
		int									port;
		std::map<std::string, std::string>	params;
		std::vector<Locations>				locations;

	public:
		Server(void);
		~Server(void);
		std::map<std::string, std::string>& getParams(void);
		std::vector<Locations>&				getLocations(void);
};

#endif
