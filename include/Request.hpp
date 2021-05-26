#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "utils.hpp"

class Request
{
	private:
		std::string							request;
		std::map<std::string, std::string>	header;

	public:
		Request(std::string request) : request(request)
		{
			std::size_t sp1 = request.find(" ");
			std::string value = request.substr(0, sp1);
			header.insert(std::pair<std::string, std::string>("method", value));

			std::size_t sp2 = request.find(" ", sp1 + 1);
			value = request.substr(sp1 + 1, sp2 - sp1 - 1);
			header.insert(std::pair<std::string, std::string>("uri", value));

			if ((sp1 = request.find("Content-Length")) != std::string::npos)
			{
				sp1 += 15;
				if ((sp2 = request.find("\r\n", sp1)) != std::string::npos)
					value = request.substr(sp1, sp2 - sp1);
				header.insert(std::pair<std::string, std::string>("content-lenght", value));
			}

			sp1 = request.find("\r\n\r\n");
			if ( request.size() != (sp1 + 4) )
			{
				sp1 += 4;
				value = request.substr(sp1, request.size() - sp1);
				header.insert(std::pair<std::string, std::string>("body", value));
			}

			if ((sp1 = request.find("X-Secret-Header-For-Test")) != std::string::npos)
			{
				sp1 += 26;
				value = "";
				if ((sp2 = request.find("\r\n", sp1)) != std::string::npos)
					value = request.substr(sp1, sp2 - sp1);
				header.insert(std::pair<std::string, std::string>("secret", value));
			}
		}
		~Request(void) {}

		std::map<std::string, std::string>		getHeader(void) { return header; }
};

#endif
