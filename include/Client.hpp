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
		struct timeval		tv;
		std::string         ip;

	public:
		Client(int serverFD) : fd(-1), serverFD(serverFD)
		{
			memset(&client, 0, sizeof(client));
			memset(&tv, 0, sizeof(tv));
			gettimeofday(&tv, 0);
			msg =	""; /*	"HTTP/1.1 200 OK\r\n \
						Server: WebServ\r\n \
						Connection: close\r\n \
						Content-Length : 169\r\n\r\n \
						<html><body><center><h2>PORCHIDDDIO</h2></center></body></html>";*/
		}
		~Client()														{ close(fd); }
		int				getFD()											{ return fd; }
		std::string&	getMsg()										{ return msg; }
		int				get_time()										{ return tv.tv_sec; }
		std::string     get_ip()                                        { return ip; }

		void	acceptClient()
		{
			int		socklen = sizeof(sockaddr_in);

			fd = accept(serverFD, (struct sockaddr*)&client, (socklen_t*)&socklen);
			fcntl(fd, F_SETFL, O_NONBLOCK);
			ip = inet_ntoa(client.sin_addr);
		}
};

#endif
