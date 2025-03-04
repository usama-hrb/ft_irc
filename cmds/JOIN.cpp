#include "../inc/Server.hpp"
#include <string>


// int	parce_channel(std::string channel)
// {
// 	if (channel[0] != '#')
// 	{
// 		std::cout << "403" << " <NICKNAME> " << channel << " :No such channel" << std::endl;
// 		return 0;
// 	}
// 	return 1;
// }

// void	Server::exec_join_cmd(std::vector<std::string> BUFFER, Server &manager, Client *c)
// {
// 	if (c->isRegistred()) {

// 	std::string		channel_name;
// 	std::cout << "HELLO\n";

// 	if (!BUFFER.empty()) {

// 		std::cout << BUFFER[0] << std::endl;
// 		channel_name = BUFFER[0];
// 		// return;
// 	}
// 	if (BUFFER.empty() || BUFFER.size() < 1)
// 	{
// 		std::string response =  "461 " + c->getNickName() +  " :Not enough parameters\r\n";
// 		sendReplay(c->getFd(), response);
// 		return;
// 	}
// 	if (!parce_channel(channel_name))
// 		return ;
// 	manager.CreatChannel(channel_name);
// 	for (int i = 0; i < _channels.size(); i++)
// 	{

// 		std::cout << "->>   "<<  _channels[i]->getName() << std::endl;
// 	}

// 	} else {
		
// 		std::string response = ERR_NOTREGISTERED(std::string("*"));
// 		sendReplay(c->getFd(), response);
// 	}
// 	// Channelmanager manager;
// }


void Server::handleJoin(Client* client, const std::vector<std::string>& params) {
    if (!client->isRegistred()) {
        sendReplay(client->getFd(), ERR_NOTREGISTERED(std::string("*")));
        return;
    }
    if (params.empty()) {
        sendReplay(client->getFd(), ERR_NEEDMOREPARAMS(std::string("JOIN")));
        return;
    }
    std::string channelName = params[0];
    if (channelName[0] != '#') {
        sendReplay(client->getFd(), ERR_NOSUCHCHANNEL(client->getNickName(), channelName));
        return;
    }

    Channel* channel = _channelManager.search_for_channel(channelName);
    if (!channel) {
        channel = _channelManager.CreatChannel(channelName);
        channel->addOperator(client); // Creator becomes operator
    }
    channel->addMember(client);

    std::string joinMsg = ":" + client->getNickName() + " JOIN :" + channelName + "\r\n";
    sendReplay(client->getFd(), joinMsg);
    channel->broadcast(joinMsg, client->getNickName());

    sendReplay(client->getFd(), RPL_NOTOPIC(client->getNickName(), channelName));
    std::string names = channel->getMemberNames();
    sendReplay(client->getFd(), RPL_NAMREPLY(client->getNickName(), channelName, names));
    sendReplay(client->getFd(), RPL_ENDOFNAMES(client->getNickName(), channelName));
}