#include "../include/utils.hpp"

Response::Response(std::map<std::string, std::string> header, Server& server) : header(header), server(server)
{
	buildResponse();
}

Response::~Response() {}

std::string Response::getResponse(void) { return response; }

std::pair<std::string, bool>	Response::isLocation(std::vector<Locations>& locations, std::string check)
{
	for (std::vector<Locations>::iterator it = locations.begin(); it != locations.end(); ++it)
		if ( ((*it).getPath() == check.substr(0, (*it).getPath().size())) && ((*it).getLocations().find("root") != (*it).getLocations().end()) )
			return std::pair<std::string, bool>(check.replace(0, (*it).getPath().size(), (*it).getLocations().find("root")->second), true);
	return std::pair<std::string, bool>("", false);
}

std::pair<std::string, bool>	Response::isIndex(std::vector<Locations>& locations, std::string check)
{
	for (std::vector<Locations>::iterator it = locations.begin(); it != locations.end(); ++it)
		if ( ((*it).getPath() == check.substr(0, (*it).getPath().size())) && ((*it).getLocations().find("root") != (*it).getLocations().end()) )
			if ( (*it).getLocations().find("index") != (*it).getLocations().end() )
				return std::pair<std::string, bool>((*it).getLocations().find("index")->second, true);
	if (server.getParams().find("index") != server.getParams().end())
		return std::pair<std::string, bool>(server.getParams().find("index")->second, true);
	return std::pair<std::string, bool>("", false);
}

std::pair<std::string, bool>	Response::isBodySize(std::vector<Locations>& locations, std::string check)
{
	for (std::vector<Locations>::iterator it = locations.begin(); it != locations.end(); ++it)
		if ( ((*it).getPath() == check.substr(0, (*it).getPath().size())) && ((*it).getLocations().find("root") != (*it).getLocations().end()) )
			if ( (*it).getLocations().find("body_size") != (*it).getLocations().end() )
				return std::pair<std::string, bool>((*it).getLocations().find("body_size")->second, true);
	if (server.getParams().find("body_size") != server.getParams().end())
		return std::pair<std::string, bool>(server.getParams().find("body_size")->second, true);
	return std::pair<std::string, bool>("", false);
}

std::pair<std::string, bool>	Response::isMethod(std::vector<Locations>& locations, std::string check)
{
	for (std::vector<Locations>::iterator it = locations.begin(); it != locations.end(); ++it)
		if (((*it).getPath() == check.substr(0, (*it).getPath().size())) &&
		    ((*it).getLocations().find("root") != (*it).getLocations().end()))
			if ((*it).getLocations().find("method_allowed") != (*it).getLocations().end())
				return std::pair<std::string, bool>((*it).getLocations().find("method_allowed")->second, true);
	return std::pair<std::string, bool>("", false);
}

bool	Response::isAutoindex(std::vector<Locations>& locations, std::string check)
{
	for (std::vector<Locations>::iterator it = locations.begin(); it != locations.end(); ++it)
		if ( ((*it).getPath() == check.substr(0, (*it).getPath().size())) && ((*it).getLocations().find("root") != (*it).getLocations().end()) )
				return (*it).getautoindex();
	return server.getautoindex();
}

void		 Response::buildResponse(void)
{
	std::string tmp = header.find("uri")->second;
	std::string method = "";
	std::pair<std::string, bool> check = isLocation(server.getLocations(), header.find("uri")->second);
	std::pair<std::string, bool> method_allowed = isMethod(server.getLocations(), header.find("uri")->second);
	autoidx = isAutoindex(server.getLocations(), header.find("uri")->second);

	if (check.second == false)
		uri = tmp.replace(0, 1, server.getParams().find("root")->second);
	else
		uri = check.first;
	if ( header.find("method") != header.end() )
		method = header.find("method")->second;

	if (check.second && method_allowed.second && (method_allowed.first.find(method) == std::string::npos))
	{
		std::string body = "";
		body = errorPage("405", "Method not Allowed");
		Headers rsp_header("405 Not Allowed", body.size(), uri, 0);
		response = rsp_header.getHeader();
		response += body;
	}
	else if (method == "HEAD")
		method_head();
	else if (method == "GET")
		method_get();
	else if (method == "POST")
		method_post();
	else if (method == "PUT")
		method_put();
}

void		Response::method_head()
{
	struct stat buffer;
	int status = 0;

	status = lstat(uri.c_str(), &buffer); // 0 se tutto ok, sennò -1
	if (status == -1)
	{
		Headers rsp_header("404 Not Found", 0, uri, 0);
		response = rsp_header.getHeader();
	}
	else if ((buffer.st_mode & S_IFMT) == S_IFREG) // FILE
	{
		Headers rsp_header("200 OK", buffer.st_size, uri, TIMESPEC);
		response = rsp_header.getHeader();
	}
	else if ((buffer.st_mode & S_IFMT) == S_IFDIR) // CARTELLA
	{
		std::pair<std::string, bool> check = isIndex(server.getLocations(), header.find("uri")->second);

		if ( check.second == true )
		{
			uri += check.first;
			if ( !(status = lstat(uri.c_str(), &buffer)) )
			{
				Headers rsp_header("200 OK", buffer.st_size, uri, TIMESPEC);
				response = rsp_header.getHeader();
			}
			else
			{
				Headers rsp_header("404 Not Found", 0, uri, 0);
				response = rsp_header.getHeader();
			}
		}
		else
		{
			Headers rsp_header("200 OK", buffer.st_size, uri, TIMESPEC);
			response = rsp_header.getHeader();
		}
	}
}

void		Response::method_get()
{
	struct stat buffer;
	int status = 0;
	std::string body = "";
	char buf[32000] = {0};

	status = lstat(uri.c_str(), &buffer); // 0 se tutto ok, sennò -1
	if (status == -1)
	{
		body = errorPage("404", "Page Not Found");
		Headers rsp_header("404 Not Found", body.size(), uri, 0);
		response = rsp_header.getHeader();
		response += body;
	}
	else if ((buffer.st_mode & S_IFMT) == S_IFREG) // FILE
	{
		int fd = open(uri.c_str(), O_RDONLY);
		int bytes = 0;

		Headers rsp_header("200 OK", buffer.st_size, uri, TIMESPEC);
		response = rsp_header.getHeader();
		while ((bytes = read(fd, buf, 32000)) > 0) {
			buf[bytes] = 0;
			body += buf;
		}
		close(fd);
		response += body;
		if (fd == -1 || bytes == -1)
		{
			body = errorPage("404", "Page Not Found");
			Headers rsp_header("404 Not Found", body.size(), uri, 0);
			response = rsp_header.getHeader();
			response += body;
		}
	}
	else if ((buffer.st_mode & S_IFMT) == S_IFDIR) // CARTELLA
	{
		std::pair<std::string, bool> check = isIndex(server.getLocations(), header.find("uri")->second);

		if ( check.second == true )
		{
			uri += check.first;
			if ( !(status = lstat(uri.c_str(), &buffer)) )
			{
				int fd = open(uri.c_str(), O_RDONLY);
				int bytes = 0;

				Headers rsp_header("200 OK", buffer.st_size, uri, TIMESPEC);
				response = rsp_header.getHeader();
				while ((bytes = read(fd, buf, 32000)) > 0) {
					buf[bytes] = 0;
					body += buf;
				}
				close(fd);
				response += body;
				if (fd == -1 || bytes == -1)
				{
					body = errorPage("404", "Page Not Found");
					Headers rsp_header("404 Not Found", body.size(), uri, 0);
					response = rsp_header.getHeader();
					response += body;
				}
			}
			else
			{
				body = errorPage("404", "Page Not Found");
				Headers rsp_header("404 Not Found", body.size(), uri, 0);
				response = rsp_header.getHeader();
				response += body;
			}
		}
		else
		{
			if (autoidx == true)	// crea autoindex
			{
				body = autoindexGenerator(uri, header.find("uri")->second);
				Headers rsp_header("200 OK", body.size(), uri, 0);
				response = rsp_header.getHeader();
				response += body;
			}
			else	// errore 403 Forbidden
			{
				body = errorPage("403", "Forbidden");
				Headers rsp_header("403 Forbidden", body.size(), uri, 0);
				response = rsp_header.getHeader();
				response += body;
			}
		}
	}
}

void		Response::method_post()
{
	PRINT("post")
}

void		Response::method_put()
{
	int fd = 0;
	std::string body = "";
	bool existed = false;


	if ((fd = open(uri.c_str(), O_RDONLY)) != -1)
		existed = true;
	close(fd);
	if (header.find("content-lenght") != header.end())
	{
		std::pair<std::string, bool> check = isBodySize(server.getLocations(), header.find("uri")->second);
		if (check.second && stoull(header.find("content-lenght")->second) > stoull(check.first))
		{
			body = errorPage("413", "Request Entity Too Large");
			Headers rsp_header("413 Request Entity Too Large", body.size(), uri, 0);
			response = rsp_header.getHeader();
			response += body;
			return ;
		}
	}
	if ( (fd = open(uri.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0655)) > 2 )
	{
		std::string body_header = header.find("body")->second;
		Headers rsp_header((!existed) ? "201 Created" : "200 OK", body.size(), uri, 0);
		response = rsp_header.getHeader();

		write(fd, body_header.c_str(), body_header.size());
	}
	else
	{
		body = errorPage("404", "Page Not Found");
		Headers rsp_header("404 Not Found", body.size(), uri, 0);
		response = rsp_header.getHeader();
		response += body;
	}
	close(fd);
}
