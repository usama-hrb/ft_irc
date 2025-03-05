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
	bool isEmpty();
    bool removeMember(std::string nickname, std::string msg);
    void broadcast(const std::string &msg, std::string senderNick);
    std::string getMemberNames(); // Add this for NAMES reply
	void		print_members(){
		std::cout << "---------------------------------------------\n";
			  std::cout << "Channel Members:\n";
    for (size_t i = 0; i < members.size(); ++i) {
        if (members[i]) // Check for null pointers
            std::cout << members[i]->getNickName() << "\n";
        else
            std::cout << "[NULL MEMBER]\n";
    }
	std::cout << "---------------------------------------------\n";
		};
};

class ChannelManager {
public:
    std::vector<Channel*> Channels;
    Channel*	search_for_channel(std::string channel_name);
	// Client*		search_for_user(std::string nickname);
    Channel*	CreatChannel(std::string channel_name);
	void		removeChannel(const std::string& channelName);
};

