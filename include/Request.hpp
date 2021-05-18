#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "utils.hpp"

class Request
{
	private:
		std::string                         request;
		std::map<std::string, std::string>  header;

	public:
		Request(std::string request) : request(request)
		{
			std::size_t sp1 = request.find(" ");
			std::string value = request.substr(0, sp1);
			header.insert(std::pair<std::string, std::string>("method", value));

			std::size_t sp2 = request.find(" ", sp1 + 1);
			value = request.substr(sp1 + 1, sp2 - sp1);
			header.insert(std::pair<std::string, std::string>("uri", value));
		}
		~Request(void) {}

        std::map<std::string, std::string>      getHeader(void) { return header; }
};

#endif