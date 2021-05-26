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
				if (std::string(envCGI[i]).substr(0, 12) == "QUERY_STRING")
				{
					av = (strlen(envCGI[i]) < 14) ? strdup("") : strdup((std::string(envCGI[i]).substr(13).c_str()));
					envCGI[i] = strdup("QUERY_STRING=") ;
				}
			/*char **echocmd = (char**)malloc(sizeof(char*) * 3);
			echocmd[0] = "/Users/cromalde/Desktop/cgi_tester";
			echocmd[1] = const_cast<char*>(uri.c_str());
			echocmd[2] = 0; */
			av.erase(std::remove_if(av.begin(), av.end(), isdigit), av.end());
			av.erase(std::remove_if(av.begin(), av.end(), isspace), av.end());
			char *echocmd[] = {"/Users/cromalde/Desktop/cgi_tester", const_cast<char*>(uri.c_str()), 0};

			std::cout << echocmd[1] << std::endl;
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
				perror(strerror(errno));
				int fd_pezza2 = open("out.txt", O_RDWR);
				std::size_t pos = 0;
				char r;

				while (read(fd_pezza2, &r, 1) > 0)
					output += r;
				/* std::cout << "-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-" << std::endl;
				std::cout << ((output == "")? "OUTPUT: ZERO": output) << std::endl;
				std::cout << "-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-" << std::endl; */
				if ((pos = output.find("Status: ")) != std::string::npos)
				{
					pos += 8;
					size_t end = output.find("\r\n");
					status = "HTTP/1.1 ";
					status += output.substr(pos, end - pos);
					status += "\r\n";
					status += "X-Secret-Header-For-Test: ";
					status += header["X-Secret-Header-For-Test"];
					status += "\r\n";
				}
				else
				{
					status = "HTTP/1.1 200 OK\r\n";
					status += "X-Secret-Header-For-Test: ";
					status += header["X-Secret-Header-For-Test"];
					status += "\r\n";
				}
				std::cout << "---------- " << header["X-Secret-Header-For-Test"] << " ----------" << std::endl;
			}
			/* if (pid == 0)
			{
				char* av = 0;
				for (int i = 0; envCGI[i]; ++i)
					if (std::string(envCGI[i]).substr(0, 12) == "QUERY_STRING")
						av = (strlen(envCGI[i]) < 14) ? strdup("") : strdup((std::string(envCGI[i]).substr(13).c_str()));
				//char *echocmd[] = {"echo", "fname=ciao&lname=cacca", NULL};
				char **echocmd = (char**)malloc(sizeof(char*) * 3);
				echocmd[0] = strdup("echo");
				echocmd[1] = av;
				echocmd[2] = 0;
				int pp[2];
				int pid2, res;

				pipe(pp);

				if ((pid2 = fork()) == 0)
				{

					close(fd[0]);
					close(fd[1]);
					dup2(pp[1], 1);
					close(pp[0]);

					if (strlen(av) < 10)
					{
						std::cerr << "execvp" << std::endl;
						res = execvp(echocmd[0], echocmd);
					}
					else
					{
						int pezza = open("cgi.txt", O_RDWR | O_CREAT, 0666);
						std::cerr << "write" << std::endl;
						res = write(pezza, av, strlen(av));
					}
				}
				else
				{
					wait(0);
					dup2(pp[0], 0);
					close(pp[1]);

					char buf = 0;

					char **av = (char**)malloc(sizeof(char*) * 3);
					av[0] = strdup(pathCGI.c_str());
					av[1] = strdup(uri.c_str());
					av[2] = 0;
					close(fd[0]);
					dup2(fd[1], 1);
					std::cerr << "sto su execve" << std::endl;
					execve(av[0], av, envCGI);
					close(fd[1]);
					exit(0);
				}
			}
			if (pid > 0)
			{
				wait(0);
				size_t pos = 0;
				char r = 0;
				close(fd[1]);
				std::cout << "Grande Padre" << std::endl;

				while (read(fd[0], &r, 1) > 0)
					output += r;
				std::cout << "-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-" << std::endl;
				std::cout << ((output == "")? "OUTPUT: ZERO": output) << std::endl;
				std::cout << "-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-" << std::endl;
				close(fd[0]);
				if ((pos = output.find("Status: ")) != std::string::npos)
				{
					pos += 8;
					size_t end = output.find("\r\n");
					status = "HTTP/1.1 ";
					status += output.substr(pos, end - pos);
					status += "\r\n";
				}
				else
					status = "HTTP/1.1 200 OK\r\n";
			} */
		}
		~CGI() {}

		std::string getOutput(void) { return (output.size() == 0) ? "\r\n" : output; }
		std::string getStatus(void) { return status; }
};

#endif
