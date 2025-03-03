#include "../inc/Needs.hpp"
#include "../inc/Server.hpp"


void Server::handleUser(Client *client, const std::vector<std::string> &params) {
	std::string response;
	if (client->isRegistred()) {
		response = ERR_ALREADYREGISTRED(std::string("*"));
		sendReplay(client->getFd(), response);
		return;
	}
	if (params.size() < 4) {
		response = ERR_NEEDMOREPARAMS(std::string("USER"));
		sendReplay(client->getFd(), response);
		return;
	}
	client->setUserName(params[0]);
	client->setRealName(params[3].substr(1));
	authenticateClient(client);
}