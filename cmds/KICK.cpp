#include "../inc/Server.hpp"

void Server::handleKick(Client* client, const std::vector<std::string>& params)
{
	std::string channel_name = params[0];
	std::string  nickname = params[1];

	int i = 0;
	if (params.size() < 3 && params.empty())
	{
		sendReplay(client->getFd(), ERR_NEEDMOREPARAMS(std::string("KICK")));
		return ;
	}
	// -----> check for my user is operator "isOperatoe()"
	if (!_channelManager.search_for_channel(channel_name))
	{
		sendReplay(client->getFd(), ERR_NOSUCHCHANNEL(client->getNickName(), channel_name));
		return ;		
	}
	for(int i = 0; (size_t)i < _channelManager.Channels.size(); i++)
	{
		if (channel_name == _channelManager.Channels[i]->getName())
		{}
	}
	if (!_channelManager.Channels[i]->removeMember(client))
	{
		std::cout << "401" << " <NICKNAME> " << nickname << " :No such nick" << std::endl;
		return ;
	}
	//brodcast
	///remove channel if is empty
}