#include "../inc/Channel.hpp"
#include "../inc/Needs.hpp"
#include "../inc/Server.hpp"
#include "../inc/Client.hpp"
#include <algorithm>

Channel::Channel(std::string new_name) : name(new_name) {}

std::string Channel::getName() {
    return name;
}

std::string Channel::getPassword() {return password;} //changed

void Channel::setPassword(std::string password) {password = password;} //changed

void Channel::addMember(Client* client) {
    if (std::find(members.begin(), members.end(), client) == members.end()) {
        members.push_back(client);
    }
}

void Channel::addOperator(Client* client) {
    if (!isOperator(client)) {
        operators.push_back(client);
    }
}

bool Channel::isOperator(Client* client) {
    return std::find(operators.begin(), operators.end(), client) != operators.end();
}

bool Channel::isMember(Client* client) {
    return std::find(members.begin(), members.end(), client) != members.end();
}

bool Channel::removeMember(Client* client) {
    for (std::vector<Client *>::iterator it = members.begin(); it != members.end(); ++it) {
        if (*it == client) {
            members.erase(it);
            return true;
        }
    }
    return false;
}

void Channel::broadcast(const std::string &msg, std::string senderNick) {
    for (size_t i = 0; i < members.size(); ++i) {
        if (members[i]->getNickName() != senderNick) {
            send(members[i]->getFd(), msg.c_str(), msg.size(), 0);
        }
    }
}

std::string Channel::getMemberNames() {
    std::string names;
    for (size_t i = 0; i < members.size(); ++i) {
        if (i > 0) names += " ";
        for (int j = 0; i < operators.size(); j++) {
            if (operators[j]->getNickName() == members[i]->getNickName()) {
                names += "@";
                break;
            }
        }
        names += members[i]->getNickName();
    }
    return names;
}

Channel* Channelmanager::search_for_channel(std::string channel_name) {
    for (size_t i = 0; i < Channels.size(); ++i) {
        if (channel_name == Channels[i]->getName()) {
            return Channels[i];
        }
    }
    return NULL;
}

Channel* Channelmanager::CreatChannel(std::string channel_name) {
    Channel* exist_channel = search_for_channel(channel_name);
    if (!exist_channel) {
        Channel* newChannel = new Channel(channel_name);
        Channels.push_back(newChannel);
        return newChannel;
    }
    return exist_channel;
}