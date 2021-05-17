#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "utils.hpp"

class Client
{
	private:
		int					fd;
		int					serverFD;
		struct sockaddr_in	client;
		std::string			msg;
	
	public:
		Client(int serverFD) : fd(-1), serverFD(serverFD), status(0)
		{
			memset(&client, 0, sizeof(client));
			msg =		"HTTP/1.1 200 OK\r\n \
						Server: WebServ\r\n \
						Connection: close\r\n \
						Content-Length : 169\r\n\r\n \
						<html><body><center><h2>PORCHIDDDIO</h2></center></body></html>";
		}
		~Client()														{ close(fd); }
		int				status;
		int				getFD()											{ return fd; }
		std::string&	getMsg()										{ return msg; }

		void	acceptClient(fd_set *read, fd_set *write, int &maxFDs)
		{
			int		socklen = sizeof(sockaddr_in);

			fd = accept(serverFD, (struct sockaddr*)&client, (socklen_t*)&socklen);
			fcntl(fd, F_SETFL, O_NONBLOCK);
			if (fd > maxFDs)
				maxFDs = fd;
		}

		enum statuses
		{
			START,
			RESPONSE,
			END
		};
};

#endif