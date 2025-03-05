#include "../inc/Server.hpp"

void Server::handleKick(Client* client, const std::vector<std::string>& params)
{
	std::string channelName = params[0];
	std::string  nickname = params[1];

	int i = 0;
	if (params.size() < 3 && params.empty())
	{
		sendReplay(client->getFd(), ERR_NEEDMOREPARAMS(std::string("KICK")));
		return ;
	}
	// -----> check for my user is operator "isOperatoe()"
	Channel* channel = _channelManager.search_for_channel(channelName);
	if (!channel)
	{
		sendReplay(client->getFd(), ERR_NOSUCHCHANNEL(client->getNickName(), channelName));
		return ;		
	}
	channel->print_members();
	std::cout << "\n\n====================================================\n\n";
	
	std::string kickMsg = ":" + client->getNickName() + " KICK " + channelName + " " + nickname + " :Kicked by operator";
	if (!channel->removeMember(nickname, kickMsg))
	{
		std::cout << "401" << " <NICKNAME> " << nickname << " :No such nick" << std::endl;
		return ;
	}
	channel->print_members();
	channel->broadcast(kickMsg, client->getNickName());  // Notify everyone in the channel

// If the kicked user is still connected, notify them
// Client* kickedClient = _clientManager.getClientByNick(nickname);
// if (kickedClient) {
//     sendReplay(kickedClient->getFd(), kickMsg);
// }

if (channel->isEmpty()) {
    _channelManager.removeChannel(channelName);
}
	// sendReplay(client->getFd(), RPL_NOTOPIC(client->getNickName(), channelName));
	// std::string names = channel->getMemberNames();
	// sendReplay(client->getFd(), RPL_NAMREPLY(client->getNickName(), channelName, names));
	// sendReplay(client->getFd(), RPL_ENDOFNAMES(client->getNickName(), channelName));
}