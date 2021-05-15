#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "utils.hpp"

class Client
{
	private:
		int					fd;
		int					serverFD;
		struct sockaddr_in	client;
	
	public:
		Client(int serverFD) : fd(-1), serverFD(serverFD)	{}
		~Client()											{ close(fd); }
		int		getFD()										{ return fd; }

		void	accept()
		{
			int		socklen = sizeof(sockaddr_in);
			fd = ::accept(serverFD, (struct sockaddr*)&client, (socklen_t*)&socklen);
			fcntl(fd, F_SETFL, O_NONBLOCK);
		}
};

#endif