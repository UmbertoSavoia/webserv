#include "../include/utils.hpp"

Config::Config(const char* filePath)
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
		throw Config::ConfigException("Config File missing");

	while (read(fd, buf, 2024))
		content += buf;

	close(fd);

	std::size_t start = 0;
	std::size_t end = 0;

	while ( (start = content.find('#')) != std::string::npos )
	{
		end = content.find('\n', start);
		content.erase(start, end - start);
	}
}

Config::~Config(void)
{
	// Print della config per DEBUG

	/* for (std::size_t i = 0; i < table.size(); ++i)
	{
		std::cout << "Server: " << i + 1 << " - " << table[i].getautoindex() << std::endl;
		for (auto i : table[i].getParams())
			std::cout << "\t" << i.first << " -- " << i.second << std::endl;
		for (auto j : table[i].getLocations())
		{
			std::cout << "\t\t-> " << j.getPath() << " - "<< j.getautoindex() << std::endl;
			for (auto k : j.getLocations())
				std::cout << "\t\t\t" << k.first << " : " << k.second << std::endl;
		}
	}*/
}

std::vector<Server>*	Config::getConfig(void)
{
	return &table;
}

void	Config::parse(void)
{
	std::pair<std::string, bool> ret;			//da riutilizzare per il ritorno di iskeywords
	bool inServer = false, inLocation = false;	//per capire in quale blocco sono
	int pos = 0;								//indice della posizione nella stringa
	int	nServ = 0;

	if (std::count(content.begin(), content.end(), '{') - std::count(content.begin(), content.end(), '}') != 0)
		throw Config::ConfigException("Mismatching Brackets!");

	while (content[pos])						//loop for servers
	{
		if (content[pos] == '}')
			++pos;
		while (isspace(content[pos]))	//skippa tutti i caratteri di spazio
			++pos;
		if (!content[pos])
			break ;
		if (content.substr(pos, 6) == "server")
			pos += 6;
		else
			throw Config::ConfigException("No 'server' block found");

		while (isspace(content[pos]))
			++pos;

		if ( content[pos] == '{' ) {
			++pos; inServer = true;
		}
		else
			 throw Config::ConfigException("No '{' after server keyword");

		while (isspace(content[pos]))
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

			while (isspace(content[pos]))
				++pos;
			ret = iskeywords(content.substr(pos), keywords, 11);
			if (ret.first == "location" && inServer == true)
			{
				//location
				while (ret.first == "location")
				{
					inLocation = true;
					pos += 8;
					while (isspace(content[pos]))
						++pos;
					Locations loc;
					loc.getPath() = getPath(content, pos);
					pos += loc.getPath().size();
					while (isspace(content[pos]) || content[pos] == '{')
						++pos;

					ret = iskeywords(content.substr(pos), keywords, 11);
					while (ret.second == true)
					{
						param = split_param(content.substr(pos));
						loc.getLocations().insert(param);
						while (content[pos] != '\n')
							++pos;
						while (isspace(content[pos]))
							++pos;
						ret = iskeywords(content.substr(pos), keywords, 11);
					}
					table[nServ].getLocations().push_back(loc);
					while (inLocation && (isspace(content[pos]) || content[pos] == '}'))
					{
						if (content[pos] == '}')
							inLocation = false;
						++pos;
					}
					while (isspace(content[pos]))
						++pos;
					ret = iskeywords(content.substr(pos), keywords, 11);
				}
			}
		}
		nServ++;
	}
	check();
}

void	Config::check(void)
{
	//	check port
	for (std::size_t j = 0; j < table.size() - 1; ++j)
	{
		std::map<std::string, std::string>::iterator itEnd = table[j].getParams().end();
		std::map<std::string, std::string>::iterator it = table[j].getParams().find("listen");
		for (std::size_t i = j + 1; i < table.size(); ++i)
		{
			std::map<std::string, std::string>::iterator itEnd_next = table[i].getParams().end();
			std::map<std::string, std::string>::iterator it_next = table[i].getParams().find("listen");
			if ((it == itEnd) || (it_next == itEnd_next) || (*it).second == (*it_next).second)
				throw Config::ConfigException("Port configuration Error !");
		}
	}

	// set autoindex
	for (std::size_t j = 0; j < table.size(); ++j)
	{
		std::map<std::string, std::string>::iterator it = table[j].getParams().find("autoindex");
		std::map<std::string, std::string>::iterator itEnd = table[j].getParams().end();

		if (it != itEnd)
			if ((*it).second == "on")
				table[j].getautoindex() = true;
		for (std::size_t i = 0; i < table[j].getLocations().size(); ++i)
		{
			std::map<std::string, std::string>::iterator itL = table[j].getLocations()[i].getLocations().find("autoindex");
			std::map<std::string, std::string>::iterator itEndL = table[j].getLocations()[i].getLocations().end();

			if (itL != itEndL)
			{
				if ((*itL).second == "on")
					table[j].getLocations()[i].getautoindex() = true;
			}
			else
			{
				if (table[j].getautoindex() == true)
					table[j].getLocations()[i].getautoindex() = true;
			}
		}
	}
}
