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
	char				buf[32000] = {0};
	std::string			message = "";
	int					bytes_read = 0;
	int					serverIDX = 0;
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
				if ((bytes_read = read((*it)->getFD(), buf, 32000)) > 0 )
				{
					buf[bytes_read] = 0;
					message += buf;
					memset(buf, 0, sizeof(buf));
				}
				else
				{
					FD_CLR((*it)->getFD(), &readfds);
					FD_CLR((*it)->getFD(), &writefds);
					delete *it;
					clients.erase(it);
					log("Read error: Client disconnected");
					break;
				}
				if (message.find("\r\n") != std::string::npos)
				{
					FD_SET((*it)->getFD(), &writefds);
					FD_CLR((*it)->getFD(), &readfds);
				}
				Request req(message);
				std::string msg = "Client " + std::to_string((*it)->getFD()) + " had send a request";
				log(msg);
				Response response(req.getHeader(), (*servers)[serverIDX], *it);
				(*it)->getMsg() = response.getResponse();

				std::cout << "-------------------------------------------------------------------------" << std::endl;
				std::cout << message << std::endl;
				std::cout << "-------------------------------------------------------------------------" << std::endl;
				std::cout << (*it)->getMsg() << std::endl;
				std::cout << "-------------------------------------------------------------------------" << std::endl;

				message.clear();
			}

			if (FD_ISSET((*it)->getFD(), &cp_writefds))
			{
				int ret = write((*it)->getFD(), (*it)->getMsg().c_str(), (*it)->getMsg().size());
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
