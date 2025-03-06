#include "../inc/Server.hpp"

// void Server::handleKick(Client* client, const std::vector<std::string>& params)
// {
// 	std::string channelName = params[0];
// 	std::string  nickname = params[1];

// 	int i = 0;
// 	if (params.size() < 3 && params.empty())
// 	{
// 		sendReplay(client->getFd(), ERR_NEEDMOREPARAMS(std::string("KICK")));
// 		return ;
// 	}
// 	// -----> check for my user is operator "isOperatoe()"
// 	Channel* channel = _channelManager.search_for_channel(channelName);
// 	if (!channel)
// 	{
// 		sendReplay(client->getFd(), ERR_NOSUCHCHANNEL(client->getNickName(), channelName));
// 		return ;		
// 	}
// 	channel->print_members();
// 	std::cout << "\n\n====================================================\n\n";
	
// 	std::string kickMsg = ":" + client->getNickName() + " KICK " + channelName + " " + nickname + " :Kicked by operator";
// 	if (!channel->removeMember(nickname, kickMsg))
// 	{
// 		std::cout << "401" << " <NICKNAME> " << nickname << " :No such nick" << std::endl;
// 		return ;
// 	}
// 	channel->print_members();
// 	channel->broadcast(kickMsg, client->getNickName());  // Notify everyone in the channel

// // If the kicked user is still connected, notify them
// // Client* kickedClient = _clientManager.getClientByNick(nickname);
// // if (kickedClient) {
// //     sendReplay(kickedClient->getFd(), kickMsg);
// // }

// if (channel->isEmpty()) {
//     _channelManager.removeChannel(channelName);

// 	std::vector<std::string> names = channel->getMemberNames();
//         std::string namesStr;
//         for (size_t j = 0; j < names.size(); j++) {
//             if (!namesStr.empty()) namesStr += " ";
//             namesStr += names[j];
//         }
//         sendReplay(client->getFd(), RPL_NAMREPLY(client->getNickName(), channelName, namesStr));
//         sendReplay(client->getFd(), RPL_ENDOFNAMES(client->getNickName(), channelName));
// }
// 	// sendReplay(client->getFd(), RPL_NOTOPIC(client->getNickName(), channelName));
// 	// std::string names = channel->getMemberNames();
// 	// sendReplay(client->getFd(), RPL_NAMREPLY(client->getNickName(), channelName, names));
// 	// sendReplay(client->getFd(), RPL_ENDOFNAMES(client->getNickName(), channelName));
// }




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
    //  = (params.size() > 2) ? params[2] : ;

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

	if (!channel->_isMember(nickname))
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