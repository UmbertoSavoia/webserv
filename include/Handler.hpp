#ifndef HANDLER_HPP
#define HANDLER_HPP

#include "utils.hpp"

class Handler
{
	private:
		std::vector<Server>*	servers;
		fd_set					master_readfds;
		fd_set					readfds;

	public:
		Handler(std::vector<Server>* servers);
		~Handler(void);
		void			init(void);
		void			serv(void);
};

#endif