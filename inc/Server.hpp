#pragma once

#include <iostream>
#include <vector> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <fcntl.h> 
#include <unistd.h> 
#include <arpa/inet.h> 
#include <poll.h> 
#include <csignal> 
#include <map>

#define RED "\e[1;31m"
#define WHI "\e[0;37m"
#define GRE "\e[1;32m"
#define YEL "\e[1;33m"
#define END "\e[0;0m"

class Server {
	int _server_fd;
    std::map<int, std::string> _clients; // fd -> buffer
    std::vector<pollfd> _poll_fds;
    std::string _password;
	public:
	//C++ CF
		Server();
		~Server();
		Server(const Server &obj);
		Server operator= (const Server &obj);

	//IRC
		void createServer(int port, std::string password);

};