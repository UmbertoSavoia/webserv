#ifndef SERVER_HPP
#define SERVER_HPP

#include "utils.hpp"
class Locations;

class Server
{
	private:
		int									fd;
		bool								autoidx;
		std::map<std::string, std::string>	params;
		std::vector<Locations>				locations;
		struct sockaddr_in					addr;
		int									reuse;

	public:
		Server(void) : fd(-1), autoidx(false), params(), locations(), reuse(1) {}
		~Server(void) {}
		bool&								getautoindex(void)		{ return autoidx; }
		std::map<std::string, std::string>&	getParams(void)			{ return params; }
		std::vector<Locations>&				getLocations(void)		{ return locations; }
		int&								getFd(void)				{ return fd; }
		void								init(void)
		{
			int port = std::stoi(((*(params.find("listen"))).second));

			addr.sin_port = htons(port);
			addr.sin_family = AF_INET;
			addr.sin_addr.s_addr = htonl(INADDR_ANY);

			fd = socket(AF_INET, SOCK_STREAM, 0);
			setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));
			bind(fd, (struct sockaddr*) &(addr), sizeof(addr));
			listen(fd, 2000);
		}
};

#endif
