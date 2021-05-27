#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "utils.hpp"

class Client
{
	private:
		int					fd;
		int					serverFD;
		struct sockaddr_in	client;
		std::string			out_msg;
		struct timeval		tv;
		std::string         ip;

	public:
		std::string			in_msg;
		Client(int serverFD) : fd(-1), serverFD(serverFD), in_msg("")
		{
			memset(&client, 0, sizeof(client));
			memset(&tv, 0, sizeof(tv));
			gettimeofday(&tv, 0);
			out_msg =	"";
		}
		~Client()						{ close(fd); }
		int				getFD()			{ return fd; }
		std::string&	getMsg()		{ return out_msg; }
		int				get_time()		{ return tv.tv_sec; }
		std::string     get_ip()        { return ip; }

		void	acceptClient()
		{
			int		socklen = sizeof(sockaddr_in);

			fd = accept(serverFD, (struct sockaddr*)&client, (socklen_t*)&socklen);
			fcntl(fd, F_SETFL, O_NONBLOCK);
			ip = inet_ntoa(client.sin_addr);
		}
};

#endif
