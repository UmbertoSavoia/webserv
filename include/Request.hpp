#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "utils.hpp"

class Request
{
	private:
		std::string request;

	public:
		Request(std::string request) : request(request)
		{
			std::cout << request;
		}
		~Request(void) {}
};

#endif