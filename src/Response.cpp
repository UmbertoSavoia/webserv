#include "../include/utils.hpp"

Response::Response(std::map<std::string, std::string> header, Server& server) : header(header), server(server)
{
    buildResponse();
}

Response::~Response() {}

std::string Response::getResponse(void) { return response; }

std::pair<std::string, bool>    Response::isLocation(std::vector<Locations>& locations, std::string check)
{
	for (std::vector<Locations>::iterator it = locations.begin(); it != locations.end(); ++it)
		if ( ((*it).getPath() == check) && ((*it).getLocations().find("root") != (*it).getLocations().end()) )
			return std::pair<std::string, bool>((*it).getLocations().find("root")->second, true);
	return std::pair<std::string, bool>("", false);
}

void        Response::buildResponse(void)
{
	std::string method = "";
	std::pair<std::string, bool> check = isLocation(server.getLocations(), header.find("uri")->second);

	if (server.getParams().find("root") != server.getParams().end())
		uri = server.getParams().find("root")->second;
	if (check.second == true)
		uri = check.first;
	else
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

void        Response::method_head()
{
	//HTTP/1.1 200 OK
	PRINT("head")
	struct stat buffer;
	int status = 0;

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
	}
}

void        Response::method_get()
{
	PRINT("get")
}

void        Response::method_post()
{
	PRINT("post")
}

void        Response::method_put()
{
	PRINT("put")
}
