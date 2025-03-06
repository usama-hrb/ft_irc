#include "../inc/Channel.hpp"
#include "../inc/Needs.hpp"
#include "../inc/Server.hpp"
#include "../inc/Client.hpp"
#include <algorithm>

Channel::Channel(std::string new_name) : name(new_name), _topic("No topic is set") {}

size_t Channel::getMemrbersNum() {return members.size();}

void Channel::setTopic(const std::string &newTopic) {
    if (newTopic.size() > 100) {
        _topic = newTopic.substr(0, 100);
    } else {
       _topic = newTopic;
    }
}


std::string Channel::getTopic() const {
    return _topic;
}

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

bool Channel::isEmpty()
{
	if (!members.size())
		return true;
	return false;
}

bool Channel::removeMember(std::string nickname, std::string msg)
{
    for (std::vector<Client*>::iterator it = members.begin(); it != members.end(); ++it) {
        if ((*it)->getNickName() == nickname)
		{
			send((*it)->getFd(), msg.c_str(), msg.size(), 0);
            members.erase(it);
            return true;
        }
    }
    return false;
}

void Channel::broadcast(const std::string &msg, std::string senderNick){
    for (size_t i = 0; i < members.size(); ++i) {
        if (members[i]->getNickName() != senderNick) {
            send(members[i]->getFd(), msg.c_str(), msg.size(), 0);
        }
    }
}

std::vector<std::string> Channel::getMemberNames() {
    std::vector<std::string> names;
    for (size_t i = 0; i < members.size(); ++i) {
        std::string name = members[i]->getNickName();
        for (size_t j = 0; j < operators.size(); j++) {
            if (operators[j]->getNickName() == members[i]->getNickName()) {
                name = "@" + name;  // Prepend @ for operators
                break;
            }
        }
        names.push_back(name);
    }
    return names;
}

Channel* ChannelManager::search_for_channel(std::string channel_name) {
    for (size_t i = 0; i < Channels.size(); ++i) {
        if (channel_name == Channels[i]->getName()) {
            return Channels[i];
        }
    }
    return NULL;
}

// Client* ChannelManager::search_for_user(std::string nickname)
// {
//     for (size_t i = 0; i < Client.size(); ++i) {
//         if (nickname == Channels[i]->getName()) {
//             return Channels[i];
//         }
//     }
//     return NULL;
// }


Channel* ChannelManager::CreatChannel(std::string channel_name) {
    Channel* exist_channel = search_for_channel(channel_name);
    if (!exist_channel) {
        Channel* newChannel = new Channel(channel_name);
        Channels.push_back(newChannel);
        return newChannel;
    }
    return exist_channel;
}

void ChannelManager::removeChannel(const std::string& channelName)
{
    for (std::vector<Channel*>::iterator it = Channels.begin(); it != Channels.end(); ++it) {
        if ((*it)->getName() == channelName) {
            delete *it;
            Channels.erase(it);
            return;
        }
    }
}
