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
	FD_ZERO(&writefds);
	FD_ZERO(&cp_readfds);
	FD_ZERO(&cp_writefds);

	for (int i = 0; i < servers->size(); ++i)
	{
		(*servers)[i].init();
		FD_SET((*servers)[i].getFd(), &readfds);
	}
}

void			Handler::serv(void)
{
	char				buf[32000] = {0};
	std::string			message = "";
	int					bytes_read = 0;
	int					maxFDs = get_max_fd(servers);
	std::string			response = "HTTP/1.1 200 OK\r\n \
						Server: WebServ\r\n \
						Connection: close\r\n \
						Content-Length : 169\r\n\r\n \
						<html><body><center><h2>PORCHIDDDIO</h2></center></body></html>";

	while (1)
	{
		cp_readfds = readfds;
		cp_writefds = writefds;

		select(maxFDs + 1, &cp_readfds, &cp_writefds, 0, 0);

		for (int i = 0; i < servers->size(); ++i)
		{
			if (FD_ISSET((*servers)[i].getFd(), &cp_readfds))
			{
				Client* tmpClient = new Client((*servers)[i].getFd());
				tmpClient->acceptClient(&readfds, &writefds, maxFDs);
				FD_SET(tmpClient->getFD(), &readfds);
				clients.push_back(tmpClient);
			}
		}

		for (std::vector<Client*>::iterator it(clients.begin()); it != clients.end(); ++it)
		{
			if (FD_ISSET((*it)->getFD(), &cp_readfds))
			{
				if ((bytes_read = read((*it)->getFD(), buf, 32000)) > 0 )
				{
					buf[bytes_read] = 0;
					message += buf;
					//if (message.size() >= 4 && message.at(message.size()-4) == '\r' && message.at(message.size()-3) == '\n' &&
					//message.at(message.size()-2) == '\r' && message.at(message.size()-1) == '\n')
					//{
					//	//break; //da pulire
					//}
					FD_SET((*it)->getFD(), &writefds);
					FD_CLR((*it)->getFD(), &readfds);
				}
				else// if (bytes_read < 0)
				{
					FD_CLR((*it)->getFD(), &readfds);
					FD_CLR((*it)->getFD(), &writefds);
					delete *it;
					clients.erase(it);
					log("Client disconnected");
					maxFDs--;
					break;
				}
				Request req(message);
				message.clear();
			}

			if (FD_ISSET((*it)->getFD(), &cp_writefds) /* && (*it)->status == Client::RESPONSE */)
			{
				//---------------------------------------------------------------------------------------
				//int ret = write((*it)->getFD(), response.c_str(), response.size());
				int ret = write((*it)->getFD(), (*it)->getMsg().c_str(), (*it)->getMsg().size());
				if (ret <= 0)
				{
					FD_CLR((*it)->getFD(), &readfds);
					FD_CLR((*it)->getFD(), &writefds);
					delete *it;
					clients.erase(it);
					log("Client disconnected");
					maxFDs--;
					break;
				}
				if ((unsigned long)ret < (*it)->getMsg().length())
					(*it)->getMsg() = (*it)->getMsg().substr(ret);
				else
				{
					FD_CLR((*it)->getFD(), &writefds);
					(*it)->getMsg().clear();
					delete *it;
					clients.erase(it);
					log("Client disconnected");
					maxFDs--;
					break;
				}
				//---------------------------------------------------------------------------------------
				//(*it)->status = Client::END;
				//FD_SET((*it)->getFD(), &readfds);
				//FD_CLR((*it)->getFD(), &writefds);
			}
		}
	}
}