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
		Config(const char* filePath);
		~Config(void);

		void					parse(void);
		void					check(void);
		std::vector<Server>*	getConfig(void);
		
		class ConfigException : public std::exception
		{
			private:
				std::string msg;
			public:
				ConfigException(std::string msg) : msg(msg) {}
				const char* what(void) const throw() override { return msg.c_str(); }
		};
};

#endif
