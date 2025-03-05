#include "../inc/Needs.hpp"
#include "../inc/Server.hpp"

void Server::handleTopic(Client* client, const std::vector<std::string>& params) {
    // Step 1: Check if the client is registered
    if (!client->isRegistred()) {
        sendReplay(client->getFd(), ERR_NOTREGISTERED(std::string("*")));
        return;
    }

    // Step 2: Validate parameters (at least channel name required)
    if (params.empty()) {
        sendReplay(client->getFd(), ERR_NEEDMOREPARAMS(std::string("TOPIC")));
        return;
    }

    std::string channelName = params[0];
    Channel* channel = _channelManager.search_for_channel(channelName);

    // Step 3: Check if the channel exists
    if (!channel) {
        sendReplay(client->getFd(), ERR_NOSUCHCHANNEL(client->getNickName(), channelName));
        return;
    }

    // Step 4: Check if the client is on the channel
    if (!channel->isMember(client)) {
        sendReplay(client->getFd(), ERR_NOTONCHANNEL(channelName));
        return;
    }

    // Step 5: Handle the command based on parameters
    if (params.size() == 1) {
        // Retrieve the current topic
        std::string topic = channel->getTopic();
        if (topic.empty()) {
            sendReplay(client->getFd(), RPL_NOTOPIC(client->getNickName(), channelName));
        } else {
            sendReplay(client->getFd(), RPL_TOPIC(channelName, topic));
        }
    } else {
        // Set a new topic
        if (!channel->isOperator(client)) {
            sendReplay(client->getFd(), ERR_CHANOPRIVSNEEDED(channelName));
            return;
        }

		std::vector<std::string> vecTopic(params.begin() + 1, params.end());
        std::string newTopic;
		for (std::vector<std::string>::iterator it = vecTopic.begin(); it != vecTopic.end(); it++)
			newTopic += *it + " ";
        channel->setTopic(newTopic);

        std::string topicMsg = ":" + client->getNickName() + " TOPIC " + channelName + " :" + newTopic + "\r\n";
        channel->broadcast(topicMsg, "");
    }
}