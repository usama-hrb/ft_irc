#include "../inc/Server.hpp"


int	parce_channel(std::string channel)
{
	if (channel[0] != '#')
	{
		std::cout << "403" << " <NICKNAME> " << channel << " :No such channel" << std::endl;
		return 0;
	}
	return 1;
}

void	Server::exec_join_cmd(std::vector<std::string> BUFFER, Server &manager, Client *c)
{
	if (c->isRegistred()) {

	std::string		channel_name;
	std::cout << "HELLO\n";

	if (!BUFFER.empty()) {

		std::cout << BUFFER[0] << std::endl;
		channel_name = BUFFER[0];
		// return;
	}
	if (BUFFER.empty() || BUFFER.size() < 1)
	{
		std::string response =  "461 " + c->getNickName() +  " :Not enough parameters\r\n";
		sendReplay(c->getFd(), response);
		return;
	}
	if (!parce_channel(channel_name))
		return ;
	manager.CreatChannel(channel_name);
	for (int i = 0; i < _channels.size(); i++)
	{

		std::cout << "->>   "<<  _channels[i]->getName() << std::endl;
	}

	} else {
		
		std::string response = ERR_NOTREGISTERED(std::string("*"));
		sendReplay(c->getFd(), response);
	}
	// Channelmanager manager;
}
