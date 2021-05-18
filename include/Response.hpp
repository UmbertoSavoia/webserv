#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "utils.hpp"

class Response
{
    private:
        std::string response;
        std::map<std::string, std::string> header;
        Server server;


    public:
        Response(std::map<std::string, std::string> header, Server& server);
        ~Response();

        void        buildResponse(void);
        std::string getResponse(void);
};

#endif