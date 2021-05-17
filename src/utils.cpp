#include "../include/utils.hpp"

bool	ft_isspace(char c)
{
	return (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r' || c == ' ');
}

std::pair<std::string, bool>	iskeywords(std::string src, std::string keywords[], int num)
{
	for (int i = 0; i < num; ++i)
		if (!(src.compare(0, keywords[i].size(), keywords[i])))
			return std::make_pair(keywords[i], true);
	return std::make_pair(std::string(""), false);
}

std::pair<std::string, std::string>	split_param(std::string src)
{
	int space = src.find(" ");
	std::string first = src.substr(0, space);

	while (ft_isspace(src[space]))
		++space;

	int lenSecond = space;
	while (src[lenSecond] != ';' && src[lenSecond] != '\n' && src[lenSecond] != ' ')
		++lenSecond;
	std::string second = src.substr(space, lenSecond - space);
	return std::make_pair(first, second);
}

std::string		getPath(std::string src, int pos)
{
	int lenStr = pos;

	while (!ft_isspace(src[lenStr]) && src[lenStr] != '{')
		++lenStr;
	return src.substr(pos, lenStr - pos);
}

int				get_max_fd(std::vector<Server>* server)
{
	int ret = 0;
	for (int i = 0; i != server->size(); ++i)
	{
		if ((*server)[i].getFd() > ret)
			ret = (*server)[i].getFd();
	}
	return ret;
}

void		log(std::string msg)
{
	struct timeval	tv;
	time_t			nowtime;
	struct tm		*nowtm;
	char			buf[100];

	gettimeofday(&tv, 0);
	nowtime = tv.tv_sec;
	nowtm = localtime(&nowtime);
	strftime(buf, sizeof(buf), "%c", nowtm);
	std::cout << "[" << buf << "] " << msg<< std::endl;
}