#include "../inc/Needs.hpp"
#include "../inc/Server.hpp"

Client* Server::findClientByNickname(const std::string& nickname) {
    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->second->getNickName() == nickname) {
            return it->second;
        }
    }
    return NULL;
}


void Server::handlePrivmsg(Client* client, const std::vector<std::string>& params) {
    if (!client->isRegistred()) {
        sendReplay(client->getFd(), ERR_NOTREGISTERED(std::string("*")));
        return;
    }
    if (params.size() < 2) {
        sendReplay(client->getFd(), ERR_NEEDMOREPARAMS(std::string("PRIVMSG")));
        return;
    }
    std::string target = params[0];
    // std::string message = params[1];
    std::vector<std::string> messages(params.begin() + 1, params.end());
    // std::cout << RED << messages[1] << END << std::endl;
    if(!messages.empty()) {
        std::string response = ERR_NEEDMOREPARAMS(std::string("PRIVMSG"));
        send(client->getFd(), response.c_str(), response.size() , 0);
        return;
    }

    if (target[0] == '#') {
        Channel* channel = _channelManager.search_for_channel(target);
        if (!channel) {
            sendReplay(client->getFd(), ERR_NOSUCHCHANNEL(client->getNickName(), target));
            return;
        }
        if (!channel->isMember(client)) {
            sendReplay(client->getFd(), ERR_NOTONCHANNEL(target));
            return;
        }
        std::string msgToSend = ":" + client->getNickName() + " PRIVMSG " + target + " :";
        for (std::vector<std::string>::iterator it = messages.begin(); it != messages.end(); ++it) {
            msgToSend += *it + " ";
        }
        msgToSend += "\r\n";
        std::cout << msgToSend << std::endl;
        channel->broadcast(msgToSend, client->getNickName());
    } else {
        Client* recipient = findClientByNickname(target);
        if (!recipient) {
            sendReplay(client->getFd(), ERR_NOSUCHNICK(client->getNickName()));
            return;
        }
        std::string msgToSend = ":" + client->getNickName() + " PRIVMSG " + target + " :";
        for (std::vector<std::string>::iterator it = messages.begin(); it != messages.end(); ++it) {
            msgToSend += *it + " ";
        }
        msgToSend += "\r\n";
        send(recipient->getFd(), msgToSend.c_str(), msgToSend.size(), 0);
    }
}