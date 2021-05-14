#include "../include/utils.hpp"

Config *p;

void		closeFDs(int)
{
	std::vector<Server>::iterator it = p->getConfig()->begin();
	std::vector<Server>::iterator ite = p->getConfig()->end();
	for ( ; it != ite; ++it)
		close(it->getFd());
	std::cout << "\b\bFine!!!" << std::endl;
	exit(0);
}

int		main(int ac, char** av)
{
	Config conf(av[1]);
	p = &conf;
	signal(SIGINT, &closeFDs);

	try {
		conf.parse();
	} catch(const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	
	Handler handler(conf.getConfig());
	handler.init();
	handler.serv();
}
