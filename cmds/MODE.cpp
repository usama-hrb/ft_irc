#include "../inc/Server.hpp"

void Server::sendNotice(Client* client, const std::string& message) {
    std::string msg = ":FT_irc" " NOTICE " + client->getNickName() + 
                     " :" + message + "\r\n";
    sendReplay(client->getFd(), msg);
}

void Server::handleMode(Client* client, const std::vector<std::string>& params)
{
	if (!client->isRegistred()) {
        sendReplay(client->getFd(), ERR_NOTREGISTERED(std::string("*")));
        return;
    }
    if (params.empty())
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
	if (params.size() == 1)
	{
		std::string modes = "";
		for (size_t i = 0; i < channel->getModes().size(); i++)
		{
			modes += channel->getModes()[i];
		}
		std::string modeMsg = ": 324 " + client->getNickName() + " " + channelName + " " + (modes.empty() ? "(no modes set)" : "+" + modes) + "\r\n";
		sendReplay(client->getFd(), modeMsg);


		return;
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
			if (params.size() > 2) {
				sendReplay(client->getFd(), ERR_INVALIDMODEPARAM(client->getNickName(), channelName, params[2]));
				return;
			}
			if (sign == '+')
			{
				channel->setInviteOnly(1);
				channel->setModes('i');
			}
			else
			{
				channel->setInviteOnly(0);
				channel->removeMode('i');
			}
		}
		else if (params[1][1] == 't')
		{
			if (sign == '+')
			{
				if (params.size() < 3) {
					sendReplay(client->getFd(), ERR_NEEDMOREPARAMS(std::string("MODE"))); return;
				}	
				std::vector<std::string> vecTopic(params.begin() + 2, params.end());
			    std::string newTopic = ":";
				for (std::vector<std::string>::iterator it = vecTopic.begin(); it != vecTopic.end(); it++)
					newTopic += *it + " ";
				channel->setTopic(newTopic);
			}
			else
			{
				if (params.size() > 2) {
					sendReplay(client->getFd(), ERR_INVALIDMODEPARAM(client->getNickName(), channelName, params[2])); return;
				}	
				channel->setTopic("No topic is set");
			}
		}
		else if (params[1][1] == 'k')
		{
			if (sign == '+')
			{
				if (params.size() < 3) {
					sendReplay(client->getFd(), ERR_NEEDMOREPARAMS(std::string("MODE"))); return;
				}
				if (params.size() > 3){
					sendReplay(client->getFd(), ERR_INVALIDMODEPARAM(client->getNickName(), channelName, params[2])); return;
				}
				channel->setPassword(params[2]);
				channel->setModes('k');
			}
			else
			{
				if (params.size() > 2){
					sendReplay(client->getFd(), ERR_INVALIDMODEPARAM(client->getNickName(), channelName, params[2])); return;
				}
				channel->setPassword("");
				channel->removeMode('k');
			}
		}
		else if (params[1][1] == 'o')
		{
			if (params.size() < 3)
			{
				sendReplay(client->getFd(), ERR_NEEDMOREPARAMS(std::string("MODE")));
				return;
			}
			if (params.size() > 3)
			{
				sendReplay(client->getFd(), ERR_INVALIDMODEPARAM(client->getNickName(), channelName, params[2]));
				return;
			}
			Client* mem = searchForUser(params[2]);
			if (sign == '+')
			{
				if (!channel->isMember(mem))
				{
					sendReplay(client->getFd(), ERR_INVALIDMODEPARAM(client->getNickName(), channelName, params[2]));
					return ;
				}
				if (channel->isOperator(mem))
				{
					sendReplay(client->getFd(), ERR_INVALIDMODEPARAM(client->getNickName(), channelName, params[2]));
					return ;
				}
				channel->addOp(mem->getNickName());
			}
			else 
			{
				
				if (!channel->isOperator(mem) || params.size() > 3)
				{
					sendReplay(client->getFd(), ERR_INVALIDMODEPARAM(client->getNickName(), channelName, params[2]));
					return ;
				}
				channel->removeOp(mem->getNickName());
			}

		}
		
		else if (params[1][1] == 'l')
		{
			if (sign == '+')
			{
				if (params[2].empty())
				{
					sendReplay(client->getFd(), ERR_NEEDMOREPARAMS(std::string("MODE")));
					return ;
				}
				if (params.size() > 3)
				{
					sendReplay(client->getFd(), ERR_INVALIDMODEPARAM(client->getNickName(), channelName, params[2]));
					return;
				}
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
				if (lim < static_cast<int>(channel->getMemrbersNum())) {
					sendNotice(client, "Channel mumbers more than limit you set!");
					return;
				}
				channel->setLimit(lim);
				channel->setModes('l');
			}
			else
			{
				if (params.size() > 2)
				{
					sendReplay(client->getFd(), ERR_INVALIDMODEPARAM(client->getNickName(), channelName, params[2]));
					return;
				}
				channel->setLimit(0);
				channel->removeMode('l');
			}
		}
		else
		{
			sendReplay(client->getFd(), ERR_NOCHANMODES(client->getNickName(), channelName));
			return;
		}
	}
	else if (!params[2].empty() && params[2] != "+sn")
	{
		sendReplay(client->getFd(), ERR_NOCHANMODES(client->getNickName(), channelName));
		return ;
	}
	if (params.size() > 2)
		channel->modeBroadcast(RPL_CHANGEMODE(channelName, params[1], params[2]));
	else
	 	channel->modeBroadcast(RPL_CHANGEMODE(channelName, params[1], ""));
}