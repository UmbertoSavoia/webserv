#ifndef CGI_HPP
#define CGI_HPP

#include "utils.hpp"

#if __linux__
	#include <sys/wait.h>
#endif

class CGI
{
	private:
		std::string	output;
		std::string	status;

	public:
		CGI(std::string pathCGI, std::string uri, char **envCGI)
		{
			pid_t pid = 0;
			Headers hds;
			output = "";

			std::string av = "";
			for (int i = 0; envCGI[i]; ++i)
			{
				if (std::string(envCGI[i]).substr(0, 12) == "QUERY_STRING")
				{
					av = (strlen(envCGI[i]) < 14) ? "" : (std::string(envCGI[i]).substr(13).c_str());
					free(envCGI[i]);
					envCGI[i] = strdup("QUERY_STRING=") ;
				}
			}
//-----------------------Stampa dell' ENV CGI, DEBUG--------------------------------------------------------------------
/* for(int i = 0; envCGI[i]; ++i)
	std::cout << envCGI[i] << std::endl; */
//----------------------------------------------------------------------------------------------------------------------
			char **echocmd = (char**)malloc(sizeof(char*) * 3);
			echocmd[0] = const_cast<char*>(pathCGI.c_str());
			echocmd[1] = const_cast<char*>(uri.c_str());
			echocmd[2] = 0;
//-----------------------Stampa dell' ENV CGI, DEBUG--------------------------------------------------------------------
/* for(int i = 0; echocmd[i]; ++i)
	std::cout << echocmd[i] << std::endl; */
//----------------------------------------------------------------------------------------------------------------------

			size_t ret =0;
			int inFile = open("cgi.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);
			ret = write(inFile, av.c_str(), av.size());
			close(inFile);

			pid = fork();
			if (pid == 0)
			{
				inFile = open("cgi.txt", O_RDONLY);
				int outFile = open("out.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);
				dup2(outFile, 1);
				dup2(inFile, 0);
				if (execve(echocmd[0], echocmd, envCGI) < 0)
					std::cerr << "errore execve" << std::endl;
				free(echocmd);
				close(outFile);
				close(inFile);
				exit(1);
			}
			else
			{
				free(echocmd);
				wait(0);
				int outFile = open("out.txt", O_RDWR);
				std::size_t pos = 0;
				char r;

				while (read(outFile, &r, 1) > 0)
					output += r;

				// STAMPA OUTPUT CGI x DEBUG
			/*	std::cout << "-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-" << std::endl;
				std::cout << ((output == "")? "OUTPUT: ZERO": output) << std::endl;
				std::cout << "-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-" << std::endl; */
				close(outFile);
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
			hds.cleanHeadersCGI(envCGI);
		}
		~CGI() {}

		std::string getOutput(void) { return (output.size() == 0) ? "\r\n" : output; }
		std::string getStatus(void) { return status; }
};

#endif
