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
		else if (params[1][1] == 't')
		{
			if (sign == '+')
			{
				if (params.size() < 3)
					sendReplay(client->getFd(), ERR_NEEDMOREPARAMS(std::string("MODE")));
				channel->setTopic(params[2]);
			}
			else
				channel->setTopic("No topic is set");
		}
		else if (params[1][1] == 'k')
		{
			if (sign == '+')
			{
				if (params.size() < 3)
					sendReplay(client->getFd(), ERR_NEEDMOREPARAMS(std::string("MODE")));
				channel->setPassword(params[2]);
			}
			else
				channel->setPassword("");
		}
		else if (params[1][1] == 'o')
		{
		}
		else if (params[1][1] == 'l')
		{
			if (sign == '+')
			{
				if (params[2].empty())
					sendReplay(client->getFd(), ERR_NEEDMOREPARAMS(std::string("MODE")));
				int lim = 0;
				std::stringstream iss(params[2]);

				iss >> lim;
				for (size_t i = 0; i < params[2].size(); i++)
				{
					if (params[2].empty() || params[2][i] < '1' || params[2][i] > '9'){
						sendReplay(client->getFd(), ERR_INVALIDMODEPARAM(client->getNickName(), channelName, params[2]));
						return;
					}
				}
				channel->setLimit(lim);
			}
			else
				channel->setLimit(0);
		}
		else
		{
			sendReplay(client->getFd(), ERR_NOCHANMODES(client->getNickName(), channelName));
			return;
		}
	}
	else
	{
		sendReplay(client->getFd(), ERR_NOCHANMODES(client->getNickName(), channelName));
		return ;
	}
	char currentSign = params[1][0];
	std::string sign_mode = params[1];
	std::string param = "";
	if (param.size() >= 3)
		std::string param = params[2];
	char mode = params[1][1];
	std::string msg = ":" + client->getNickName() + " MODE " + channelName + " " + currentSign + mode + "\r\n";
	channel->modeBroadcast(msg);
	// sendReplay(client->getFd(), RPL_CHANNELMODEIS(client->getNickName(), channelName, sign_mode, param));
}