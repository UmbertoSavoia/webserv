#ifndef CGI_HPP
#define CGI_HPP

#include "utils.hpp"

#if __linux__
	#include <sys/wait.h>
#endif

class CGI
{
	private:
		std::string                         output;
		std::string                         status;

	public:
		CGI(std::string pathCGI, std::string uri, char **envCGI, std::map<std::string, std::string>& header)
		{
			pid_t pid = 0;
			int fd[2];
			output = "";

			std::string av = "";
			for (int i = 0; envCGI[i]; ++i)
			{
				if (std::string(envCGI[i]).substr(0, 12) == "QUERY_STRING")
				{
					av = (strlen(envCGI[i]) < 14) ? strdup("") : strdup((std::string(envCGI[i]).substr(13).c_str()));
					envCGI[i] = strdup("QUERY_STRING=") ;
				}
			}
//----------------------------------------------------------------------------------------------------------------------
for(int i = 0; envCGI[i]; ++i)
	std::cout << envCGI[i] << std::endl;
//----------------------------------------------------------------------------------------------------------------------
			char **echocmd = (char**)malloc(sizeof(char*) * 3);
			echocmd[0] = const_cast<char*>(pathCGI.c_str());
			echocmd[1] = const_cast<char*>(uri.c_str());
			echocmd[2] = 0;
//----------------------------------------------------------------------------------------------------------------------
for(int i = 0; echocmd[i]; ++i)
	std::cout << echocmd[i] << std::endl;
//----------------------------------------------------------------------------------------------------------------------
			av.erase(std::remove_if(av.begin(), av.end(), [](int c){return (isspace(c) || isdigit(c) || c == 'a');}), av.end());

			size_t ret =0;
			int fd_pezza3 = open("cgi.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);
			ret = write(fd_pezza3, av.c_str(), av.size());
			close(fd_pezza3);

			pid = fork();
			if (pid == 0)
			{
				fd_pezza3 = open("cgi.txt", O_RDONLY);
				int pezzatotale = open("out.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);
				dup2(pezzatotale, 1);
				dup2(fd_pezza3, 0);
				if (execve(echocmd[0], echocmd, envCGI) < 0)
					std::cerr << "errore execve" << std::endl;
				exit(1);
			}
			else
			{
				wait(0);
				int fd_pezza2 = open("out.txt", O_RDWR);
				std::size_t pos = 0;
				char r;

				while (read(fd_pezza2, &r, 1) > 0)
					output += r;
				/*std::cout << "-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-" << std::endl;
				std::cout << ((output == "")? "OUTPUT: ZERO": output) << std::endl;
				std::cout << "-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-" << std::endl;*/
				if ((pos = output.find("Status: ")) != std::string::npos)
				{
					pos += 8;
					size_t end = output.find("\r\n");
					status = "HTTP/1.1 ";
					status += output.substr(pos, end - pos);
					status += "\r\n";
				}
				else
				{
					status = "HTTP/1.1 200 OK\r\n";
				}
			}
		}
		~CGI() {}

		std::string getOutput(void) { return (output.size() == 0) ? "\r\n" : output; }
		std::string getStatus(void) { return status; }

};


#endif
