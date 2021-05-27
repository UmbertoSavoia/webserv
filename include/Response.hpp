#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "utils.hpp"

class Response
{
	private:
		std::string							response;
		std::map<std::string, std::string>	header;
		Server								server;
		Client*								client;
		std::string							uri;
		bool								autoidx;
		std::string							cgi_path;
		std::string							error_page;


	public:
		Response(std::map<std::string, std::string> header, Server& server, Client* client);
		~Response();

		void							buildResponse(void);
		std::pair<std::string, bool>	isLocation(std::vector<Locations>& locations, std::string check);
		std::pair<std::string, bool>	isIndex(std::vector<Locations>& locations, std::string check);
		bool							isAutoindex(std::vector<Locations>& locations, std::string check);
		std::pair<std::string, bool>	isBodySize(std::vector<Locations>& locations, std::string check);
		std::pair<std::string, bool>	isMethod(std::vector<Locations>& locations, std::string check);
		std::pair<std::string, bool>    isCGI(std::vector<Locations>& locations, std::string check, std::string tmpURI);


		std::string						getResponse(void);
		void							method_head();
		void							method_get();
		void							method_post();
		void							method_put();
};

#endif
