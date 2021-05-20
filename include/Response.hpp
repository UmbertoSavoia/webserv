#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "utils.hpp"

class Response
{
    private:
        std::string                         response;
        std::map<std::string, std::string>  header;
        Server                              server;
        std::string                         uri;
        bool                                autoidx;


    public:
        Response(std::map<std::string, std::string> header, Server& server);
        ~Response();

        void                            buildResponse(void);
        std::pair<std::string, bool>    isLocation(std::vector<Locations>& locations, std::string check);
        std::pair<std::string, bool>	isIndex(std::vector<Locations>& locations, std::string check);
        bool                            isAutoindex(std::vector<Locations>& locations, std::string check);
        std::string                     getResponse(void);
		void                            method_head();
		void                            method_get();
		void                            method_post();
		void                            method_put();
};

#endif
