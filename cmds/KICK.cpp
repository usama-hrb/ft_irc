#include "../inc/Server.hpp"

void Server::handleKick(Client* client, const std::vector<std::string>& params)
{
	if (!client->isRegistred()) {
        sendReplay(client->getFd(), ERR_NOTREGISTERED(std::string("*")));
        return;
    }
    // Check for minimum required parameters (channel and nickname)
    if (params.size() < 2)
    {
        sendReplay(client->getFd(), ERR_NEEDMOREPARAMS(std::string("KICK")));
        return;
    }
	std::string reason;
    std::string channelName = params[0];
    std::string nickname = params[1];
	std::vector<std::string> messages(params.begin() + 2, params.end());
	if (params.size() > 2) {
		for (std::vector<std::string>::iterator it = messages.begin(); it != messages.end(); ++it) {
            reason += *it + " ";
        }
	} else {
		reason = "Kicked by operator";
	}
    // Find the channel
    Channel* channel = _channelManager.search_for_channel(channelName);
    if (!channel)
    {
        sendReplay(client->getFd(), ERR_NOSUCHCHANNEL(client->getNickName(), channelName));
        return;
    }
    // Verify the client is an operator
    if (!channel->isOperator(client))
    {
        sendReplay(client->getFd(), ERR_CHANOPRIVSNEEDED(channelName));
        return;
    }

	if (!channel->checkForClient(nickname))
	{
		sendReplay(client->getFd(), ERR_NOSUCHNICK(client->getNickName()));
		return ;
	}
    // Construct the kick message in proper IRC format
    std::string kickMsg = ":" + client->getNickName() + " KICK " + channelName + " " + nickname + " " + reason + "\r\n";

    // Remove the member from the channel
    channel->removeMember(nickname, kickMsg);

    // Broadcast the kick message to all channel members
    channel->broadcast(kickMsg, "");


    std::vector<std::string> names = channel->getMemberNames();
    std::string namesStr;
    for (size_t i = 0; i < names.size(); i++)
    {
        if (!namesStr.empty()) namesStr += " ";
        namesStr += names[i];
    }
    std::string namesReply = RPL_NAMREPLY(client->getNickName(), channelName, namesStr);
    std::string endOfNames = RPL_ENDOFNAMES(client->getNickName(), channelName);

    // Remove the channel if it’s empty
    if (channel->isEmpty())
    {
        _channelManager.removeChannel(channelName);
    }
}