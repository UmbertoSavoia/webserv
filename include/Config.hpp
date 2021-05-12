#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "utils.hpp"
class Server;

class Config
{
	private:
		std::string			content;
		std::vector<Server>	table;

		std::string keywords[11];

	public:
		Config(char* filePath);
		~Config(void);

		void	parse(void);

		class ConfigException : public std::exception
		{
			public:
				const char* what(void) const throw();
		};
};

#endif
