#ifndef CGI_HPP
#define CGI_HPP

#include "utils.hpp"

class CGI
{
	private:
		std::string                         output;
		std::string                         status;

	public:
		CGI(std::string pathCGI, std::string uri, char **envCGI)
		{
			pid_t pid = 0;
			int fd[2];
			output = "";

			pipe(fd);
			pid = fork();

			if (pid == 0)
			{


				char *echocmd[] = {"echo", "lname=ciao&fname=ca", NULL};
				int pp[2];
				int pid2, res;

				pipe(pp);

				if ((pid2 = fork()) != 0)
				{
					close(fd[0]);
					close(fd[1]);
					dup2(pp[1], 1);
					close(pp[0]);

					res = execvp(echocmd[0], echocmd);
				}
				else
				{
					dup2(pp[0], 0);
					close(pp[1]);

					char **av = (char**)malloc(sizeof(char*) * 3);
					av[0] = strdup(pathCGI.c_str());
					av[1] = strdup(uri.c_str());
					av[2] = 0;
					close(fd[0]);
					std::cout << av[0] << std::endl << av[1] << std::endl;
					dup2(fd[1], 1);
					execve(pathCGI.c_str(), av, envCGI);
					close(fd[1]);
					exit(0);
				}

				//close(pp[1]);



			}
			if (pid > 0)
			{
				size_t pos = 0;
				wait(0);
				char r = 0;
				close(fd[1]);
				while (read(fd[0], &r, 1) > 0)
					output += r;
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
			}
		}
		~CGI() {}

		std::string getOutput(void) { return output; }
		std::string getStatus(void) { return status; }
};

#endif
