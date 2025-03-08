#include "../inc/Server.hpp"

void Server::handleMode(Client* client, const std::vector<std::string>& params)
{

	if (!client->isRegistred()) {
        sendReplay(client->getFd(), ERR_NOTREGISTERED(std::string("*")));
        return;
    }
    if (params.empty() || params.size() < 2)
	{
        sendReplay(client->getFd(), ERR_NEEDMOREPARAMS(std::string("MODE")));
        return;
    }

	std::string channelName = params[0];
	Channel *channel = _channelManager.search_for_channel(channelName);
	if (!channel)
	{
		sendReplay(client->getFd(), ERR_NOSUCHCHANNEL(client->getNickName(), channelName));
		return ;
	}
	if (!channel->isOperator(client))
    {
        sendReplay(client->getFd(), ERR_CHANOPRIVSNEEDED(channelName));
        return;
    }
	if (params[1].size() == 2 && (params[1][0] == '-' || params[1][0] == '+'))
	{
		char sign = params[1][0];
		if (params[1][1] == 'i')
		{
			if (sign == '+')
				channel->setInviteOnly(1);
			else
				channel->setInviteOnly(0);
		}
		else if (!params[2].empty() && params[1][1] == 't')
		{
			if (sign == '+')
				channel->setTopic(params[2]);
			else
				channel->setTopic("No topic is set");
		}
		else if (params[1][1] == 'k')
		{
		}
		else if (params[1][1] == 'o')
		{
		}
		else if (params[1][1] == 'l')
		{
			int lim = 0;
			std::stringstream iss(params[2]);

			iss >> lim;
			for (size_t i = 0; i < params[2].size(); i++)
			{
				if (params[2].empty() || params[2][i] < '0' || params[2][i] > '9'){
					sendReplay(client->getFd(), ERR_NOCHANMODES(client->getNickName(), channelName));
					return;
				}
			}
			if (sign == '+')
				channel->setLimit(lim);
			else
				channel->setLimit(0);
		}
		else
			sendReplay(client->getFd(), ERR_NOCHANMODES(client->getNickName(), channelName));
	}
}