#include "../include/Config.hpp"

const char* Config::ConfigException::what(void) const throw()
{
	return ("Error: file configuration");
}

Config::Config(char* filePath)
{
	int fd = 0;
	char buf[2024] = {0};

	if ( (fd = open(filePath, O_RDONLY)) == -1 )
		throw Config::ConfigException();

	while (read(fd, buf, 2024))
		content += buf;

	close(fd);
}

Config::~Config(void)
{
	//std::cout << content << std::endl;
}

void	Config::parse(void)
{
	std::pair<std::string, bool> ret;			//da riutilizzare per il ritorno di iskeywords
	bool inServer = false, inLocation = false;	//per capire in quale blocco sono
	int pos = 0;								//indice della posizione nella stringa

	while (ft_isspace(content[pos]))	//skippa tutti i caratteri di spazio
		++pos;

	ret = iskeywords(content.substr(pos), keywords, 10);	//check se la parola incontrata è una keywords
	if ( ret.second == true && ret.first == "server" )
		pos += ret.first.size();
	else
		throw Config::ConfigException();

	while (ft_isspace(content[pos]))
		++pos;

	if ( content[pos] == '{' ) {
		++pos; inServer = true;
	}
	else
		throw Config::ConfigException();

	while (ft_isspace(content[pos]))
		++pos;

	ret = iskeywords(content.substr(pos), keywords, 10);
	if (ret.second == true && ret.first != "server" && ret.first != "location")
	{
		std::cout << split_param(content.substr(pos)).first << std::endl;
		std::cout << split_param(content.substr(pos)).second << std::endl;
	}
}
