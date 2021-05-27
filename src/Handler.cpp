#include "../include/utils.hpp"

Handler::Handler(std::vector<Server>* servers) : servers(servers) {}

Handler::~Handler(void)
{
	/* for (std::size_t i = 0; i < servers->size(); ++i)
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
	} */
}

void			Handler::init(void)
{
	FD_ZERO(&readfds);
	FD_ZERO(&writefds);
	FD_ZERO(&cp_readfds);
	FD_ZERO(&cp_writefds);
	std::string log_msg;

	for (std::size_t i = 0; i < servers->size(); ++i)
	{
		(*servers)[i].init();
		FD_SET((*servers)[i].getFd(), &readfds);
		log_msg = "Opening port :" + (*servers)[i].getParams().find("listen")->second;
		log(log_msg);
	}
}

void			Handler::serv(void)
{
	char				*buf = (char*)malloc(100000001);
	int					bytes_read = 0;
	int					serverIDX = 0;
	bool 				isBrowser = false;
	struct	timeval		tv;

	while (1)
	{
		cp_readfds = readfds;
		cp_writefds = writefds;

		select(FD_SETSIZE, &cp_readfds, &cp_writefds, 0, 0);

		for (std::size_t i = 0; i < servers->size(); ++i)
		{
			if (FD_ISSET((*servers)[i].getFd(), &cp_readfds))
			{
				Client* tmpClient = new Client((*servers)[i].getFd());
				tmpClient->acceptClient();
				FD_SET(tmpClient->getFD(), &readfds);
				clients.push_back(tmpClient);
				serverIDX = i;
			}
		}

		for (std::vector<Client*>::iterator it(clients.begin()); it != clients.end(); ++it)
		{
			if (FD_ISSET((*it)->getFD(), &cp_readfds))
			{
				if ((bytes_read = recv((*it)->getFD(), buf, 100000, 0)) > 0 )
				{
					buf[bytes_read] = 0;
					(*it)->in_msg += buf;
					memset(buf, 0, 100001);
				}
				else if (bytes_read == 0)
				{
					FD_CLR((*it)->getFD(), &readfds);
					FD_CLR((*it)->getFD(), &writefds);
					delete *it;
					clients.erase(it);
					log("Read error: Client disconnected");
					break;
				}
				std::size_t s = 0;
				isBrowser = false;
				if ((s = (*it)->in_msg.find("\r\n\r\n")) != std::string::npos)
				{
					std::size_t body = 0;
					if ((body = (*it)->in_msg.find("Content-Length")) != std::string::npos)
					{
						isBrowser = true;
						body = strtoul((*it)->in_msg.substr(body + 15, (*it)->in_msg.find("\r\n", body) - body).c_str(), 0, 0);
					}
					if (((*it)->in_msg.substr(0, 5).find("PUT") != std::string::npos ||
						(*it)->in_msg.substr(0, 5).find("POST") != std::string::npos) )
					{
						if ((isBrowser && ((*it)->in_msg.substr(s + 4).size() >= body)) || ((*it)->in_msg.substr(s + 4).find("\r\n\r\n") != std::string::npos))
						{
							FD_SET((*it)->getFD(), &writefds);
							FD_CLR((*it)->getFD(), &readfds);
						}
						else
							break;
					}
					else {
						FD_SET((*it)->getFD(), &writefds);
						FD_CLR((*it)->getFD(), &readfds);
					}
				}
				else
					break ;
				Request req((*it)->in_msg);
				std::string msg = "Client " + std::to_string((*it)->getFD()) + " had send a request";
				log(msg);
				Response response(req.getHeader(), (*servers)[serverIDX], *it);
				(*it)->getMsg() = response.getResponse();

				// STAMPA DELLA RICHIESTA E DELLA RISPOSTA, PER DEBUG
			/*	std::cout << "-------------------------------------------------------------------------" << std::endl;
				std::cout << "\033[32m" << message << "\033[0m" << std::endl;
				std::cout << "-------------------------------------------------------------------------" << std::endl;
				std::cout << "\033[36m" << (*it)->getMsg() << "\033[0m" << std::endl;
				std::cout << "-------------------------------------------------------------------------" << std::endl; */

				(*it)->in_msg.clear();
			}

			if (FD_ISSET((*it)->getFD(), &cp_writefds))
			{
				int ret = send((*it)->getFD(), (*it)->getMsg().c_str(), (*it)->getMsg().size(), 0);
				if (ret <= 0)
				{
					FD_CLR((*it)->getFD(), &readfds);
					FD_CLR((*it)->getFD(), &writefds);
					delete *it;
					clients.erase(it);
					log("Write error: Client disconnected");
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
					break;
				}
			}
			memset(&tv, 0, sizeof(tv));
			gettimeofday(&tv, 0);
			if ((FD_ISSET((*it)->getFD(), &cp_readfds)) && (*it)->get_time() - tv.tv_sec > 10)
			{
				FD_CLR((*it)->getFD(), &readfds);
				FD_CLR((*it)->getFD(), &writefds);
				delete *it;
				clients.erase(it);
				log("Client disconnected by Time");
				break;
			}
		}
	}
}
