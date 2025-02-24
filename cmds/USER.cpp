#include "../inc/Needs.hpp"
#include "../inc/Server.hpp"


void Server::handleUser(Client *client, const std::vector<std::string> &params) {
	if (client->isRegistred()) {
		std::cout << "Already registred!!!!!\n";
		return;
	}
	if (params.size() < 4) {
		std::cout << "Username if small!!!\n";
		return;
	}
	client->setUserName(params[0]);
	client->setRealName(params[3].substr(1));
	authenticateClient(client);
}
