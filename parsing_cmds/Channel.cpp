/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: souaouri <souaouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 12:28:13 by souaouri          #+#    #+#             */
/*   Updated: 2025/03/03 14:31:39 by souaouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel* Channelmanager::search_for_channel(std::string channel_name)
{
	for(int i = 0; (size_t)i < Channels.size(); i++)
	{
		if (channel_name == Channels[i]->getName())
			return (Channels[i]);
	}
	return (NULL);
}

User* Channelmanager::search_for_user(std::string nickname)
{
    for (size_t i = 0; i < global_users.size(); ++i)
	{
        if (global_users[i]->getNick() == nickname)
			return global_users[i];
    }
    return NULL;
}

Channel*	Channelmanager::CreatChannel(std::string channel_name)
{
	Channel* exist_channel  = search_for_channel(channel_name);
	if (!exist_channel)
	{
		Channel* newChannel = new Channel(channel_name);
		newChannel->addMember("<NICKNAME>"); //
		newChannel->addOperator("<NICKNAME>"); //
		Channels.push_back(newChannel);
		return newChannel;
	}
	//channel.addUser(user);
	// exist_channel->addMember("<NICKNAME>");
	return exist_channel;
}

void Channel::addMember(std::string nickname)
{
	User *user = new User(nickname);
    if (std::find(members.begin(), members.end(), user) == members.end()) {
        members.push_back(user);
    }
	//std::cout << "--------mem__________>  " << members[0]->getNick() << std::endl;
	// std::cout << "--------oper__________>" << operators[0]->getNick();
}

void Channel::addOperator(std::string nickname)
{
	User *user = new User(nickname);
    if (!isOperator(user)) {
        operators.push_back(user);
    }
}

bool Channel::isOperator(User *user)
{
    return std::find(operators.begin(), operators.end(), user) != operators.end();
}

bool Channel::isMember(std::string user)
{
    for(int i = 0; (size_t)i < members.size(); i++){
		if (members[i]->getNick() == user)
			return true;
	}
	return false;
}

bool Channel::removeMember(std::string nickname)
{
	for (int i = 0; (size_t)i < members.size(); i++)
	{
		if (members[i]->getNick() == nickname)
		{
			members.erase(members.begin() + i);
			return (true);
		}
	}
	return (false);
}

std::string Channel::getName()
{
	return (name);
}

Channel::Channel(std::string new_name){
	name = new_name;
}

void Channel::broadcast(const std::string &msg, std::string sender)
{
    for (size_t i = 0; i < members.size(); ++i) {
        if (members[i]->getNick() != sender) {
            members[i]->sendMessage(msg);
        }
    }
}