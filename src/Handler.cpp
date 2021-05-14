#include "../include/utils.hpp"

Handler::Handler(std::vector<Server>* servers) : servers(servers) {}

Handler::~Handler(void)
{
	for (std::size_t i = 0; i < servers->size(); ++i)
	{
		std::cout << "Server: " << i + 1 << " - " << (*servers)[i].getautoindex() << std::endl;
		for (auto i : (*servers)[i].getParams())
			std::cout << "\t" << i.first << " -- " << i.second << std::endl;
		for (auto j : (*servers)[i].getLocations())
		{
			std::cout << "\t\t-> " << j.getPath() << " - "<< j.getautoindex() << std::endl;
			for (auto k : j.getLocations())
				std::cout << "\t\t\t" << k.first << " : " << k.second << std::endl;
		}
	}
}

void			Handler::init(void)
{
	FD_ZERO(&readfds);
	FD_ZERO(&master_readfds);

	for (int i = 0; i < servers->size(); ++i)
	{
		(*servers)[i].init();
		FD_SET((*servers)[i].getFd(), &master_readfds);
	}
}

void			Handler::serv(void)
{
	struct sockaddr_in	client;
	int					fd_client;
	int					socklen = sizeof(sockaddr_in);
	char				buf[1025] = {0};
	std::string			message = "";
	int					bytes_read = 0;

	while (1)
	{
		readfds = master_readfds;
		select(FD_SETSIZE, &readfds, 0, 0, 0);

		for (int i = 0; i < servers->size(); ++i)
		{
			if (FD_ISSET((*servers)[i].getFd(), &readfds))
			{
				fd_client = accept((*servers)[i].getFd(), (struct sockaddr*)&client, (socklen_t*)&socklen);
				break;
			}
		}

		while ((bytes_read = recv(fd_client, buf, 1024, 0)) > 0 )
		{
			buf[bytes_read] = 0;
			message += buf;
			if (message.size() >= 4 && message.at(message.size()-4) == '\r' && message.at(message.size()-3) == '\n' &&
			message.at(message.size()-2) == '\r' && message.at(message.size()-1) == '\n')
			{
				break; //da pulire
			}
		}
		Request req(message);

		//---------------------------------------------------------------------------------------
		std::string response = "HTTP/1.1 200 OK\r\nServer: WebServ\r\nConnection: close\r\n\r\n";
					response += "<html><body><center><h2>PORCO DIO</h2></center></body></html>";
					send(fd_client, response.c_str(), response.size(), 0);
		//---------------------------------------------------------------------------------------
		close(fd_client);
		message.clear();
	}
}