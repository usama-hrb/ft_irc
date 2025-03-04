#pragma once

#include "Needs.hpp"
#include "Client.hpp"
#include <vector>

class Channel {
private:
    std::string name;
    std::string password; //changed
    std::vector<Client*> members;
    std::vector<Client*> operators;

public:
    Channel(std::string new_name);
    std::string getName();
	std::string getPassword();//changed
	void setPassword(std::string password);//changed
    void addMember(Client* client);
    void addOperator(Client* client);
    bool isOperator(Client* client);
    bool isMember(Client* client);
    bool removeMember(Client* client);
    void broadcast(const std::string &msg, std::string senderNick);
    std::string getMemberNames(); // Add this for NAMES reply
};

class Channelmanager {
public:
    std::vector<Channel*> Channels;
    Channel* search_for_channel(std::string channel_name);
    Channel* CreatChannel(std::string channel_name);
};