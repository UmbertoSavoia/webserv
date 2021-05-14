#include "../include/utils.hpp"

int		main(int ac, char** av)
{
	try {
		Config conf(av[1]);
		conf.parse();
	} catch(const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}
