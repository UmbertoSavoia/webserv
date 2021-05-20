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
		if ( ((*it).getPath() == check) && ((*it).getLocations().find("root") != (*it).getLocations().end()) )
			return std::pair<std::string, bool>((*it).getLocations().find("root")->second, true);
	return std::pair<std::string, bool>("", false);
}

std::pair<std::string, bool>	Response::isIndex(std::vector<Locations>& locations, std::string check)
{
	for (std::vector<Locations>::iterator it = locations.begin(); it != locations.end(); ++it)
		if ( ((*it).getPath() == check) && ((*it).getLocations().find("root") != (*it).getLocations().end()) )
			return std::pair<std::string, bool>((*it).getLocations().find("index")->second, true);
	return std::pair<std::string, bool>("", false);
}

void		 Response::buildResponse(void)
{
	std::string method = "";
	std::pair<std::string, bool> check = isLocation(server.getLocations(), header.find("uri")->second);

	if (server.getParams().find("root") != server.getParams().end())
		uri = server.getParams().find("root")->second;
	if (check.second == true)
		uri = check.first;
	else if (check.second == false && header.find("uri")->second != "/")
		uri += header.find("uri")->second;
	if ( header.find("method") != header.end() )
		method = header.find("method")->second;
	if (method == "HEAD")
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
	//HTTP/1.1 200 OK
/* 	PRINT("head")
	struct stat buffer;
	int status = 0;

	// Costruzione Header
	status = lstat(uri.c_str(), &buffer); // 0 se tutto ok, sennò -1
	if (status == -1)
	{
		Headers rsp_header("HTTP/1.1 404 Not Found", 0, uri, 0);
		response = rsp_header.getHeader();
	}
	else
	{
		Headers rsp_header("HTTP/1.1 200 OK", buffer.st_size, uri, buffer.st_mtimespec.tv_sec - 7200);
		response = rsp_header.getHeader();
	} */
}

void		Response::method_get()
{/*
	PRINT("get")
	struct stat buffer;
	int status = 0;
	int fd = 0;
	bool flag = false;

	// Costruzione Header
	status = lstat(uri.c_str(), &buffer); // 0 se tutto ok, sennò -1
	if (status == -1)
	{
		Headers rsp_header("HTTP/1.1 404 Not Found", 0, uri, 0);
		response = rsp_header.getHeader();
	}
	else
	{
		Headers rsp_header("HTTP/1.1 200 OK", buffer.st_size, uri, buffer.st_mtimespec.tv_sec - 7200);
		response = rsp_header.getHeader();
		flag = true;
	}

	// Attaccare body alla response
	std::pair<std::string, bool> check = isIndex(server.getLocations(), header.find("index")->second);
	if (flag)
	{
		if (check.second == true)
		{
			std::string path = uri + check.first;
			char buf = 0;
			int	ret = 0;
			if ((fd = open(path.c_str(), fd, O_RDONLY)) == -1)
				flag = false;
			else
			{
				while ((ret = read(fd, &buf, 1)) > 0)
					response += buf;
				if (ret == -1)
					flag = false; // ERRORE DI READ, INSERIRE ECCEZIONE ADEGUATA
			}
		}
		else
		{
			std::string path = uri + "index.html";
			char buf = 0;
			int	ret = 0;
			if ((fd = open(path.c_str(), fd, O_RDONLY)) == -1)
				flag = false;
			else
			{
				while ((ret = read(fd, &buf, 1)) > 0)
					response += buf;
				if (ret == -1)
					flag = false; // ERRORE DI READ, INSERIRE ECCEZIONE ADEGUATA
			}
			PRINT(response)
		}
	}
	if (!flag)
	{
		PRINT("STO NEL 404")
		size_t pos = 0;
		char buf = 0;
		int	ret = 0;
		if ((pos = response.find("200 OK")) != std::string::npos)
		{
			response.replace(pos, 6, "404 Not Found");
		}
		if ((fd = open("/Users/cromalde/Desktop/webserv/www/error.html", fd, O_RDONLY)) == -1)
			flag = false; // ERRORE DI OPEN, INSERIRE ECCEZIONE ADEGUATA
		else
		{
			while ((ret = read(fd, &buf, 1)) > 0)
				response += buf;
			if (ret == -1)
				flag = false; // ERRORE DI READ, INSERIRE ECCEZIONE ADEGUATA
			PRINT(response)
		}
	} */
}

void		Response::method_post()
{
/* 	PRINT("post")
	struct stat buffer;
	int status = 0;

	// Costruzione Header
	status = lstat(uri.c_str(), &buffer); // 0 se tutto ok, sennò -1
	if (status == -1)
	{
		Headers rsp_header("HTTP/1.1 404 KO", 0, uri, 0);
		response = rsp_header.getHeader();
	}
	else
	{
		Headers rsp_header("HTTP/1.1 200 OK", buffer.st_size, uri, buffer.st_mtimespec.tv_sec - 7200);
		response = rsp_header.getHeader();
	} */
}

void		Response::method_put()
{
/* 	PRINT("put")
	struct stat buffer;
	int status = 0;

	// Costruzione Header
	status = lstat(uri.c_str(), &buffer); // 0 se tutto ok, sennò -1
	if (status == -1)
	{
		Headers rsp_header("HTTP/1.1 404 KO", 0, uri, 0);
		response = rsp_header.getHeader();
	}
	else
	{
		Headers rsp_header("HTTP/1.1 200 OK", buffer.st_size, uri, buffer.st_mtimespec.tv_sec - 7200);
		response = rsp_header.getHeader();
	} */
}
