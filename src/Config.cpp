#include "../include/utils.hpp"

const char* Config::ConfigException::what(void) const throw()
{
	return ("Error: file configuration");
}

Config::Config(char* filePath)
{
	std::string words[11] = {"listen", "location", "server_name",
							"index", "body_size", "error_page", "method",
							"root", "autoindex", "exec_cgi", "extension_cgi"};
	for (int i = 0; i < 11; ++i)
		keywords[i] = words[i];
	table.reserve(2);
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
	for (std::size_t i = 0; i < table.size(); ++i)
	{
		std::cout << "Server: " << i+1 << std::endl;
		for (auto i : table[i].getParams())
		{
			std::cout << "\t" << i.first << " -- " << i.second << std::endl;
		}
		for (auto j : table[i].getLocations())
		{
			std::cout << "\t\t-> " << j.getPath() << std::endl;
			for (auto k : j.getLocations())
				std::cout << "\t\t\t" << k.first << " : " << k.second << std::endl;
		}
	}
}

void	Config::parse(void)
{
	std::pair<std::string, bool> ret;			//da riutilizzare per il ritorno di iskeywords
	bool inServer = false, inLocation = false;	//per capire in quale blocco sono
	int pos = 0;								//indice della posizione nella stringa
	int	nServ = 0;

	while (content[pos])						//loop for servers
	{
		if (content[pos] == '}')
			++pos;
		while (ft_isspace(content[pos]))	//skippa tutti i caratteri di spazio
			++pos;
		if (!content[pos])
			break ;
		if (content.substr(pos, 6) == "server")
			pos += 6;
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

		ret = iskeywords(content.substr(pos), keywords, 11);
		Server x;
		table.push_back(x);
		while (ret.second == true && ret.first != "server" && inLocation == false && inServer == true)
		{
			std::pair<std::string, std::string> param = split_param(content.substr(pos));

			table[nServ].getParams().insert(param);

			while (content[pos] != '\n')
				++pos;

			while (ft_isspace(content[pos]))
				++pos;
			ret = iskeywords(content.substr(pos), keywords, 11);
			if (ret.first == "location" && inServer == true)
			{
				//location
				while (ret.first == "location")
				{
					inLocation = true;
					pos += 8;
					while (ft_isspace(content[pos]))
						++pos;
					Locations loc;
					loc.getPath() = getPath(content, pos);
					pos += loc.getPath().size();
					while (ft_isspace(content[pos]) || content[pos] == '{')
						++pos;

					ret = iskeywords(content.substr(pos), keywords, 11);
					while (ret.second == true)
					{
						param = split_param(content.substr(pos));
						loc.getLocations().insert(param);
						while (content[pos] != '\n')
							++pos;
						while (ft_isspace(content[pos]))
							++pos;
						ret = iskeywords(content.substr(pos), keywords, 11);
					}
					table[nServ].getLocations().push_back(loc);
					while (inLocation && (ft_isspace(content[pos]) || content[pos] == '}'))
					{
						if (content[pos] == '}')
							inLocation = false;
						++pos;
					}
					while (ft_isspace(content[pos]))
						++pos;
					ret = iskeywords(content.substr(pos), keywords, 11);
				}
			}
		}
		nServ++;
	}
}