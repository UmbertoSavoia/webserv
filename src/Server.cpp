#include "../include/Server.hpp"

Server::Server(void) :
	socket_fd(-1), port(-1), server_name(), index(), body_size(), error_page(),
	method(), root() {}

Server::~Server(void) {}
