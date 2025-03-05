#include "../inc/Server.hpp"

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


std::vector<std::string> split(const std::string& str, char delimiter)
{
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end;

    while ((end = str.find(delimiter, start)) != std::string::npos)
	{
        tokens.push_back(str.substr(start, end - start));
        start = end + 1;
	}
    tokens.push_back(str.substr(start));

    return tokens;
}

void Server::handleJoin(Client* client, const std::vector<std::string>& params) {
	
    // if (!client->isRegistred()) {
    //     sendReplay(client->getFd(), ERR_NOTREGISTERED(std::string("*")));
    //     return;
    // }
    if (params.empty()) {
        sendReplay(client->getFd(), ERR_NEEDMOREPARAMS(std::string("JOIN")));
        return;
    }
	std::vector<std::string> channels = split(params[0], ',') ;
	for(int i = 0; i < channels.size(); i++)
	{
		std::string channelName = channels[i];
		if (channelName[0] != '#') {
			sendReplay(client->getFd(), ERR_NOSUCHCHANNEL(client->getNickName(), channelName));
			continue;
		}
		Channel* channel = _channelManager.search_for_channel(channelName);
		int hasChannel = 0;
		if (!channel)
		{
			hasChannel = 1;
			if (params.size() > i + 1 && !params[i + 1].empty())
			{
				channel->setPassword(params[i + 1]);
			}
			channel = _channelManager.CreatChannel(channelName);
			channel->addOperator(client); // Creator becomes operator
		}
		std::string password = "";
		if (!params[i + 1].empty())
			password = params[i + 1];
		// std::string walo = "---------------> : " + password + "\n";
		// sendReplay(client->getFd(), walo);
		// std::cout << "+++++++++++++ " << channel->getPassword();
		// std::cout << "------------- " << password; 
		// if (channel->getPassword() != password && !hasChannel)
		// {
		// 	// std::cout << "++++++ in +++++++\n";
		// 	std::string msg = "ERR_PASSWORD\n"; //
		// 	sendReplay(client->getFd(), msg);
		// }
		channel->addMember(client);

		std::string joinMsg = ":" + client->getNickName() + " JOIN :" + channelName + "\r\n";
		sendReplay(client->getFd(), joinMsg);
		channel->broadcast(joinMsg, client->getNickName());

		if (hasChannel)
		{
			sendReplay(client->getFd(), RPL_NOTOPIC(client->getNickName(), channelName));
			std::string names = channel->getMemberNames();
			sendReplay(client->getFd(), RPL_NAMREPLY(client->getNickName(), channelName, names));
			sendReplay(client->getFd(), RPL_ENDOFNAMES(client->getNickName(), channelName));
		}
	}
}

void Server::handleList(Client* client, const std::vector<std::string>& params) {
	for (int i = 0; i < _channelManager.Channels.size(); i++)
	{
		sendReplay(client->getFd(), _channelManager.Channels[i]->getName() + "\n");
	}
	std::string response = RPL_LISTEND(std::string(""));
	sendReplay(client->getFd(), response);
}