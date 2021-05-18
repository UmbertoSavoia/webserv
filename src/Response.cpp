#include "../include/utils.hpp"

Response::Response(std::map<std::string, std::string> header, Server& server) : header(header), server(server)
{
    buildResponse();
}

Response::~Response() {}

std::string Response::getResponse(void) { return response; }

void        Response::buildResponse(void)
{

}