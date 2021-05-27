#include "../include/utils.hpp"


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

	while (isspace(src[space]))
		++space;

	int lenSecond = space;
	while (src[lenSecond] != ';' && src[lenSecond] != '\n' /*&& src[lenSecond] != ' '*/)
		++lenSecond;
	std::string second = src.substr(space, lenSecond - space);
	trim(first);
	trim(second);
	return std::make_pair(first, second);
}

std::string		getPath(std::string src, int pos)
{
	int lenStr = pos;

	while (!isspace(src[lenStr]) && src[lenStr] != '{')
		++lenStr;
	return src.substr(pos, lenStr - pos);
}

int				get_max_fd(std::vector<Server>* server)
{
	int ret = 0;
	for (std::size_t i = 0; i != server->size(); ++i)
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

void	trim(std::string &str)
{
	str.erase(0, str.find_first_not_of(" "));
	while (str.back() == ' ')
		str.erase(str.back());
}

std::string	errorPage(std::string numCode, std::string description, std::string error_page)
{
	std::string ret = "";
	if (error_page.size() > 0)
	{
		char buf[51] = {0};
		int fd = open(error_page.c_str(), O_RDONLY);
		int r = 0;
		while ((r = read(fd, buf, 50)) > 0)
		{
			buf[r] = 0;
			ret += buf;
			memset(buf, 0, 51);
		}
	}
	else
	{
		ret = "	<html> <head></head> <body> <center> \
				<h1> " + numCode + " <br> " + description + " </h1> \
				</center> </body> </html> \r\n\r\n";
	}
	return ret;
}

std::string	autoindexGenerator(std::string path, std::string request)
{
	DIR *dir = opendir(path.c_str());
	struct dirent *dp;
	std::string ret = "<html><head><title>Index of ";
				ret += path + "</title></head><body><h1>Index of " + path + "</h1><hr>";

	while ((dp = readdir(dir)) != 0)
	{
		if (dp->d_name[0] == '.')
			continue ;
		ret += "<a href=\"." + request + "/" + dp->d_name + "\">" + dp->d_name + "</a><br>";
	}
	closedir(dir);
	ret += "<hr></body></html>";
	return ret;
}
