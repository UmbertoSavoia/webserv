#ifndef HEADERS_HPP
#define HEADERS_HPP

#include "utils.hpp"

class Headers
{
	private:
		std::string headers;
		struct timeval tv;
		struct tm *nowtm;
		time_t nowtime;
		char buf[200];
		std::string accept_charsets;
		std::string accept_language;
		std::string allow;
		std::string authorization;
		std::string content_language;
		std::string content_lenght;
		std::string content_location;
		std::string content_type;
		std::string date;
		std::string host;
		std::string last_modified;
		std::string location;
		std::string referer;
		std::string retry_after;
		std::string server;
		std::string transfer_encoding;
		std::string user_agent;
		std::string www_authenticate;

	public:
		Headers(std::string status, int content_len, std::string uri, long timespec)
		{
			headers = "";
			headers += status + " \r\n";
			headers += accept_charsets = "Accept-Charset: iso-8859-5, unicode-1-1;q=0.8 \r\n";
			headers += accept_language = "Content-Language: it, en \r\n";
			headers += content_lenght = "Content-Length: " + std::to_string(content_len) + "\r\n";
			headers += content_location = "Content-Location: " + uri + " \r\n";
			headers += content_type = "Content-Type: text/html; charset=UTF-8 \r\n";
			gettimeofday(&tv, 0);
			nowtime = tv.tv_sec - 7200;
			nowtm = localtime(&nowtime);
			memset(buf, 0, sizeof(buf));
			strftime(buf, sizeof(buf), "Date: %a, %d %b %G %X GMT \r\n", nowtm);
			headers += date = buf;
			headers += host = "Host: localhost\r\n";
			nowtime = timespec;
			nowtm = localtime(&nowtime);
			memset(buf, 0, sizeof(buf));
			strftime(buf, sizeof(buf), "Last-Modified: %a, %d %b %G %X GMT \r\n", nowtm);
			headers += last_modified = buf;
			headers += location = "Location: " + uri + "\r\n";
			headers += referer = "Referer: http://localhost/" + uri + "\r\n";
			headers += retry_after = "Retry-After: 120\r\n";
			headers += server = "Server: WebServer\r\n";
			headers += transfer_encoding = "Transfer-Encoding: gzip\r\n";
			//www_authenticate = "Entrapureamicomio";  DA SISTEMARE QUANDO AVREMO ERRORE 401;
			headers += "\r\n";
		}
		~Headers() {}
		std::string     getHeader(void) { return headers; }
};

#endif
