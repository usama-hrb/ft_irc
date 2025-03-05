#include "../inc/Server.hpp"

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
	
    if (!client->isRegistred()) {
        sendReplay(client->getFd(), ERR_NOTREGISTERED(std::string("*")));
        return;
    }
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
			channel->addOperator(client);
		}
		std::string password = "";
		// if (!params[i + 1].empty())
		// 	password = params[i + 1];
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

		std::vector<std::string> names = channel->getMemberNames();
        std::string namesStr;
        for (size_t j = 0; j < names.size(); j++) {
            if (!namesStr.empty()) namesStr += " ";
            namesStr += names[j];
        }
		if (channel->getTopic().empty()) {
			sendReplay(client->getFd(), RPL_NOTOPIC(client->getNickName(), channelName));
		} else {
			sendReplay(client->getFd(), RPL_TOPIC(channelName, channel->getTopic()));
		}
        sendReplay(client->getFd(), RPL_NAMREPLY(client->getNickName(), channelName, namesStr));
        sendReplay(client->getFd(), RPL_ENDOFNAMES(client->getNickName(), channelName));
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