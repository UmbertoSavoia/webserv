#ifndef HEADERS_HPP
#define HEADERS_HPP

#include "utils.hpp"

class Headers
{
	private:
		std::string headers_http;
		std::map<std::string, std::string> headers_cgi;
		struct timeval tv;
		struct tm *nowtm;
		time_t nowtime;
		char buf[200];

	public:
		Headers(void) {}
		~Headers() {}

		void    headersHead(std::string status)
		{
			headers_http = "HTTP/1.1 ";
			headers_http += status + " \r\n";
			headers_http += "Server: WebServer\r\n";
			headers_http += "\r\n";
		}

		void    headersHTTP(std::string status, int content_len, std::string uri, long timespec)
		{
			headers_http = "HTTP/1.1 ";
			headers_http += status + " \r\n";
			headers_http += "Accept-Charset: iso-8859-5, unicode-1-1;q=0.8 \r\n";
			headers_http += "Content-Language: it, en \r\n";
			headers_http += "Content-Length: " + std::to_string(content_len) + "\r\n";
			headers_http += "Content-Location: " + uri + " \r\n";
			headers_http += "Content-Type: text/html; charset=UTF-8 \r\n";
			gettimeofday(&tv, 0);
			nowtime = tv.tv_sec;
			nowtm = gmtime(&nowtime);
			memset(buf, 0, sizeof(buf));
			strftime(buf, sizeof(buf), "Date: %a, %d %b %G %X GMT \r\n", nowtm);
			headers_http += buf;
			headers_http += "Host: localhost\r\n";
			nowtime = timespec;
			nowtm = gmtime(&nowtime);
			memset(buf, 0, sizeof(buf));
			strftime(buf, sizeof(buf), "Last-Modified: %a, %d %b %G %X GMT \r\n", nowtm);
			headers_http += buf;
			headers_http += "Location: " + uri + "\r\n";
			headers_http += "Referer: http://localhost/" + uri + "\r\n";
			headers_http += "Retry-After: 120\r\n";
			headers_http += "Server: WebServer\r\n";
			headers_http += "Transfer-Encoding: identity\r\n";
			headers_http += "\r\n";
		}

		void	headersCGI(std::map<std::string, std::string>& header, Client* client, Server& server, std::string uri)
		{
			// META VARIABILI DA MANDARE AL CGI
/*			AUTH_TYPE
			CONTENT_LENGTH
			CONTENT_TYPE
			GATEWAY_INTERFACE
			PATH_INFO
			PATH_TRANSLATED
			QUERY_STRING
			REMOTE_ADDR
			REMOTE_IDENT
			REMOTE_USER
			REQUEST_METHOD
			REQUEST_URI
			SCRIPT_NAME
			SERVER_NAME
			SERVER_PORT
			SERVER_PROTOCOL
			SERVER_SOFTWARE */
			std::size_t found = 0;

			headers_cgi["AUTH_TYPE"] = "";

			if (header.find("body") != header.end())
				headers_cgi["CONTENT_LENGTH"] = std::to_string(header.find("body")->second.size());
			else
				headers_cgi["CONTENT_LENGTH"] = "";

			if (header.find("method")->second == "GET")
				headers_cgi["CONTENT_TYPE"] = "text/html";
			else
				headers_cgi["CONTENT_TYPE"] = "application/x-www-form-urlencoded";

			headers_cgi["GATEWAY_INTERFACE"] = "CGI/1.1";

			if ((uri.find(".bla") != std::string::npos) || (uri.find("post_body") != std::string::npos))
			{
				headers_cgi["PATH_INFO"] = header.find("uri")->second;
				headers_cgi["PATH_TRANSLATED"] = header.find("uri")->second;
			}  else {
				headers_cgi["PATH_INFO"] = uri;
				headers_cgi["PATH_TRANSLATED"] = uri;
			}

			if (header.find("method")->second == "POST")
			{
				if (header.find("body") != header.end())
				{
					headers_cgi["QUERY_STRING"] = header.find("body")->second;
					headers_cgi["QUERY_STRING"].erase(std::remove_if(headers_cgi["QUERY_STRING"].begin(), headers_cgi["QUERY_STRING"].end(), [](int c){return (isspace(c) || isdigit(c) || c == 'a');}), headers_cgi["QUERY_STRING"].end() );
					headers_cgi["CONTENT_LENGTH"] = std::to_string(headers_cgi["QUERY_STRING"].size());
				}
				else
					headers_cgi["QUERY_STRING"] = "";
			}
			else
			{
				if (header.find("method")->second == "GET" && (found = header.find("uri")->second.find('?') != std::string::npos))
					headers_cgi["QUERY_STRING"] = header.find("uri")->second.substr(found, header.find("uri")->second.size() - found);
				else
					headers_cgi["QUERY_STRING"] = "";
			}

			headers_cgi["REMOTE_ADDR"] = client->get_ip();

			headers_cgi["REMOTE_IDENT"] = "";
			headers_cgi["REMOTE_USER"] = "";

			headers_cgi["REQUEST_METHOD"] = header.find("method")->second;
			headers_cgi["REQUEST_URI"] = header.find("uri")->second;

			if ((found = uri.find('?')) != std::string::npos)
				headers_cgi["SCRIPT_NAME"] = uri.substr(0, found);
			else
				headers_cgi["SCRIPT_NAME"] = uri;

			headers_cgi["SERVER_NAME"] = "localhost";
			headers_cgi["SERVER_PORT"] = server.getParams().find("listen")->second;
			headers_cgi["SERVER_PROTOCOL"] = "HTTP/1.1";
			headers_cgi["SERVER_SOFTWARE"] = "WebServ";

			if (header.find("secret") != header.end())
				headers_cgi["HTTP_X_SECRET_HEADER_FOR_TEST"] = header.find("secret")->second;
			else
				headers_cgi["HTTP_X_SECRET_HEADER_FOR_TEST"] = "";
		}

		std::string		getHeaderHTTP(void) { return headers_http; }

		char**		getHeaderCGI(void)
		{
			int i = 0;
			char** ret = (char**)malloc(sizeof(char*) * (headers_cgi.size() + 1));

			for (std::map<std::string, std::string>::iterator it = headers_cgi.begin(); it != headers_cgi.end(); ++it, ++i)
				ret[i] = strdup(const_cast<char*>(std::string(it->first + "=" + it->second).c_str()));
			ret[i] = 0;
			return ret;
		}

		void		cleanHeadersCGI(char **headers)
		{
			for (int i = 0; headers[i]; ++i)
				free(headers[i]);
			free(headers);
		}

		std::string		getCGIbody_size(void) { return headers_cgi["CONTENT_LENGTH"]; }

};

#endif
