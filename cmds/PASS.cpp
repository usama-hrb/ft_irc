#include "../inc/Needs.hpp"
#include "../inc/Server.hpp"


void Server::handlePass(Client *client, const std::vector<std::string> &params) {
	if (client->hasPassword()) {
		std::cout << "already registred!!!!\n";
		return;
	}
	if (params.empty()) {
		std::cout << "params are not good!!!!!!!\n";
		return;
	}
	if (params[0] != _password) {
		std::cout << "incorrect password!!!!!\n";
		closeClient(client->getFd());
		return;
	}
	client->setPassword(true);
}