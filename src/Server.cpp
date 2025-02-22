#include "../inc/Server.hpp"

void Server::createSock() {
	_serverFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverFd == -1)
		throw (std::runtime_error("socket error : " + std::string(strerror(errno))));
	int opt = 1;
	if (setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
		close(_serverFd), throw (std::runtime_error("setsockopt error : " + std::string(strerror(errno))));
	if (fcntl(_serverFd, F_SETFL, O_NONBLOCK) == -1)
		close(_serverFd), throw (std::runtime_error("fcntl error : " + std::string(strerror(errno))));
}

void Server::setupAddr() {
	memset(&_serverAddress, 0, sizeof(_serverAddress));
	_serverAddress.sin_family = AF_INET;
	_serverAddress.sin_addr.s_addr = INADDR_ANY;
	_serverAddress.sin_port = htons(_port);
}

void Server::bindSock() {
	if (bind(_serverFd, (struct sockaddr *)&_serverAddress, sizeof(_serverAddress)) == -1)
		close(_serverFd), throw (std::runtime_error("bind error : " + std::string(strerror(errno))));
}

void Server::startListening() {
	if (listen(_serverFd, SOMAXCONN) == -1)
		close(_serverFd), throw (std::runtime_error("listen error : " + std::string(strerror(errno))));
}

void Server::handleNewConnection() {

	struct sockaddr_in 	clientAddress;
	socklen_t			clientLen = sizeof(clientAddress);
	int 				clientFd = accept(_serverFd, (struct sockaddr*)&clientAddress, &clientLen);
	if (clientFd == -1) {
		std::cerr << "accept error : " << strerror(errno) << std::endl;
		return;
	}
	if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1) {
		close(clientFd), std::cerr << "fcntl error when add new client : " << strerror(errno) << std::endl;
		return; 
	}
	_pollFds.push_back((pollfd){clientFd, POLLIN, 0});
	std::cout << "New connection from " << inet_ntoa(clientAddress.sin_addr) << ":" << ntohs(clientAddress.sin_port) << std::endl;
}

void Server::handleClientData(int clientFd) {
    char buffer[1337];
    ssize_t bytesRead = recv(clientFd, buffer, sizeof(buffer) - 1, 0);

    if (bytesRead <= 0) {
        if (bytesRead == 0) {
            std::cout << "Client disconnected" << std::endl;
        } else {
            std::cerr << "recv error: " << strerror(errno) << std::endl;
        }
        closeClient(clientFd);
        return;
    }

    buffer[bytesRead] = '\0';
	std::string message(buffer);
    std::cout << "Received: " << buffer << std::endl;

	Client* client = _clients[clientFd];
	
	std::istringstream iss(message);
	std::string commandLine;
	while(std::getline(iss, commandLine)) {
		commandLine = commandLine.substr(0, commandLine.find_last_not_of("\r\n") + 1);
		processCommand(client, commandLine);
	}
}

void Server::processCommand(Client* client, const std::string& command) {
	std::istringstream iss(command);
	std::vector<std::string> params;
	std::string cmd;

	iss >> cmd;
	for (std::string param; iss >> param; )
		params.push_back(param);
	if (cmd == "PASS") handlePass(client, params);
	else if (cmd == "NICK") handleNick(client, params);
	else if (cmd == "USER") handleUser(client, params);
	
}

void Server::closeClient(int clientFd) {
    for (std::vector<pollfd>::iterator it = _pollFds.begin(); it != _pollFds.end(); ++it) {
        if (it->fd == clientFd) {
            _pollFds.erase(it);
            break;
        }
    }
    close(clientFd);
}

Server::Server(int port, const std::string &password) : _port(port), _password(password), _serverFd(-1), _running(false) {
	createSock();
	setupAddr();
	bindSock();
	startListening();
}

Server::~Server() {
    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        close(it->first);
        delete it->second;
    }
    close(_serverFd);
}

Server::Server(const Server &obj) {

}

Server &Server::operator= (const Server &obj) {

	return (*this);
}

void Server::run() {
	_running = true;
	_pollFds.push_back((pollfd){_serverFd, POLLIN, 0});

	while (_running) {
		int state = poll(&_pollFds[0], _pollFds.size(), -1);

		if (state < 0 && errno != EINTR)
			throw (std::runtime_error("poll error : " + std::string(strerror(errno))));
		for (size_t i = 0; i < _pollFds.size(); i++) {
			if (_pollFds[i].fd == _serverFd)
				handleNewConnection();
			else
			 	handleClientData(_pollFds[i].fd);
		}
	}
}