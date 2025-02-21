#include "../inc/Server.hpp"

Server::Server() {

}

Server::~Server() {

}

Server::Server(const Server &obj) {

}

Server Server::operator= (const Server &obj) {

	return (*this);
}

void Server::createServer(int port, std::string password) {
	_password = password;
	sockaddr_in address = {};

	std::cout << "post is => " << port << "\npassword is => "<< password << "\n";

	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = INADDR_ANY;
	memset(&address.sin_zero, 0, 8);
	// init the socket
	int socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketFd < 0)
		throw (std::runtime_error("Error : Socket not open!"));
	int nBind = 0;
	nBind = bind(socketFd, (sockaddr*)&address, sizeof(address));
	if (nBind < 0)
		throw (std::runtime_error("Error : Binding the socket!"));
	else
	 	std::cout << "ok all is good haha\n";

	nBind = listen(socketFd, SOMAXCONN);
	if (nBind < 0)
		throw (std::runtime_error("Error : listen is failed!"));
	else
		std::cout << GRE << "Lestning..." << END << std::endl ;


	// init the env for sockaddr structure
	// bind the socket
	// Listen to request from client
	// Keeo waiting for new requests and proceed as per the request

}