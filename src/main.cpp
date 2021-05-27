#include "../include/utils.hpp"

Config *p;

void		closeFDs(int)
{
	std::vector<Server>::iterator it = p->getConfig()->begin();
	std::vector<Server>::iterator ite = p->getConfig()->end();
	for ( ; it != ite; ++it)
		close(it->getFd());
	std::cout << "\b\b";
	log("Shutting down servers...");
	unlink("cgi.txt");
	unlink("out.txt");
	exit(0);
}

int		main(int ac, char** av)
{
	char r = 0;
	std::string file;

	if (ac < 2)
	{
		std::cout << "No input file specified, do you want the default config [Y/n]?" << std::endl;
		r = std::getchar();
		if (r == 'Y' || r == 'y')
			file = "config/config_default.conf";
		else
			return 0;
	} else {
		file = av[1];
	}
	Config conf(file.c_str());
	p = &conf;
	signal(SIGINT, &closeFDs);
	signal(SIGTERM, &closeFDs);

	try {
		log("Parsing of configuration file");
		conf.parse();
	} catch(const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	Handler handler(conf.getConfig());
	handler.init();
	log("Server is starting...");
	handler.serv();
}
