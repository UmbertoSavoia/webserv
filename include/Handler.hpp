#ifndef HANDLER_HPP
#define HANDLER_HPP

#include "utils.hpp"

class Client;

class Handler
{
	private:
		std::vector<Server>*	servers;
		std::vector<Client*>	clients;
		fd_set					readfds;
		fd_set					writefds;
		fd_set					cp_readfds;
		fd_set					cp_writefds;

	public:
		Handler(std::vector<Server>* servers);
		~Handler(void);
		void			init(void);
		void			serv(void);
};

#endif