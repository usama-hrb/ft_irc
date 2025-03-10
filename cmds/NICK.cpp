#include "../inc/Needs.hpp"
#include "../inc/Server.hpp"


void Server::handleNick(Client* client, const std::vector<std::string>& params) {
    if (params.empty()) {
        sendReplay(client->getFd(), ERR_NONICKNAMEGIVEN(client->getNickName()));
        return;
    }
    std::string nickname = params[0];
    if (!isValidNickname(nickname)) {
        sendReplay(client->getFd(), ERR_ERRONEUSNICKNAME(client->getNickName(), nickname));
        return;
    }
    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->second->getNickName() == nickname) {
            sendReplay(client->getFd(), ERR_NICKNAMEINUSE(client->getNickName(), nickname));
            return;
        }
    }
    client->setNickName(nickname);
	authenticateClient(client);
}

bool Server::isValidNickname(const std::string& nickname) {
    if (nickname.empty() || nickname.length() > 9)
        return false;
    const std::string special = "-[]\\`^{}";
    for (size_t i = 0; i < nickname.length(); ++i) {
        char c = nickname[i];
        if (i == 0 && !isalpha(c) && special.find(c) == std::string::npos)
            return false;
        if (!isalnum(c) && special.find(c) == std::string::npos)
            return false;
    }
    return true;
}