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

// void Server::handlePrivmsg(Client* client, const std::vector<std::string>& params) {
//     if (!client->isRegistred()) {
//         sendReplay(client->getFd(), ERR_NOTREGISTERED(std::string("*")));
//         return;
//     }
//     if (params.size() < 2) {
//         sendReplay(client->getFd(), ERR_NEEDMOREPARAMS(std::string("PRIVMSG")));
//         return;
//     }
    
//     std::string target = params[0];

//     std::string recvmessage = params[1];
//     // for (size_t i = 1; i < params.size(); i++) {  
//     //     recvmessage += params[i] + " ";
//     // }
//     // if (!recvmessage.empty())
//     //     recvmessage.erase(recvmessage.size() - 1);

//     // if (!recvmessage.empty() && recvmessage[0] == ':')
//     //     recvmessage = recvmessage.substr(1);

//     // if (recvmessage.find("DCC SEND") != std::string::npos) {
//     //     bool isCtcp = !recvmessage.empty() && recvmessage.front() == '\x01' && recvmessage.back() == '\x01';
//     //     if (!isCtcp) {
//     //         recvmessage = "\x01" + recvmessage + "\x01";
//     //     }
//     // }


//     std::string senderInfo = client->getNickName() + "!" + client->getUserName() + "@" + client->getClientIp();
//     std::string msgToSend = ":" + senderInfo + " PRIVMSG " + target + " :" + recvmessage + "\r\n";

//     if (target[0] == '#') {
//         Channel* channel = _channelManager.search_for_channel(target);
//         if (!channel) {
//             sendReplay(client->getFd(), ERR_NOSUCHCHANNEL(client->getNickName(), target));
//             return;
//         }
//         if (!channel->isMember(client)) {
//             sendReplay(client->getFd(), ERR_NOTONCHANNEL(target));
//             return;
//         }
//         channel->broadcast(msgToSend, client->getNickName());
//     } else {
//         Client* recipient = findClientByNickname(target);
//         if (!recipient) {
//             sendReplay(client->getFd(), ERR_NOSUCHNICK(client->getNickName()));
//             return;
//         }
//         send(recipient->getFd(), msgToSend.c_str(), msgToSend.size(), 0);
//     }
// }



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
    std::string recvmessage = params[1];

    // Check if the message is a CTCP/DCC command
    bool isCtcp = !recvmessage.empty() && recvmessage[0] == '\x01' && recvmessage.back() == '\x01';
    if (recvmessage.find("DCC SEND") != std::string::npos && !isCtcp) {
        recvmessage = "\x01" + recvmessage + "\x01";
    }

    std::string senderInfo = client->getNickName() + "!" + client->getUserName() + "@" + client->getClientIp();
    std::string msgToSend = ":" + senderInfo + " PRIVMSG " + target + " :" + recvmessage + "\r\n";

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
        channel->broadcast(msgToSend, client->getNickName());
    } else {
        Client* recipient = findClientByNickname(target);
        if (!recipient) {
            sendReplay(client->getFd(), ERR_NOSUCHNICK(client->getNickName()));
            return;
        }
        send(recipient->getFd(), msgToSend.c_str(), msgToSend.size(), 0);
    }
}