#ifndef SERVER_HPP
#define SERVER_HPP

#include "utils.hpp"

class Server
{
	private:
		int			socket_fd;
		int			port;
		std::string	server_name;
		std::string	index;			//per ora si accetta un solo index
		std::size_t	body_size;
		std::string	error_page;
		std::string	method;			//nelle location o qui?
		std::string root;

	public:
		Server(void);
		~Server(void);
};

#endif
