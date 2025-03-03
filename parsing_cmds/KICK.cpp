/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: souaouri <souaouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:14:23 by souaouri          #+#    #+#             */
/*   Updated: 2025/03/02 15:49:36 by souaouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.hpp"

void exec_kick_cmd(std::vector<std::string> BUFFER, Channelmanager& manager)
{
	std::string channel_name = BUFFER[1];
	std::string  nickname = BUFFER[2];

	int i = 0;
	if (BUFFER.size() < 3)
	{
		std::cout << "461" << " <NICKNAME> " <<  BUFFER[0] << " :Not enough parameters" << std::endl;
		return ;
	}
	// -----> check for my user is operator "isOperatoe()"
	if (!manager.search_for_channel(channel_name))
	{
		std::cout << "403" << " <NICKNAME> " << channel_name << " :No such channel" << std::endl;
		return ;		
	}
	for(int i = 0; (size_t)i < manager.Channels.size(); i++)
	{
		if (channel_name == manager.Channels[i]->getName())
		{}
	}
	if (!manager.Channels[i]->removeMember(nickname))
	{
		std::cout << "401" << " <NICKNAME> " << nickname << " :No such nick" << std::endl;
		return ;
	}
	//brodcast
	///remove channel if is empty
}

#include "Channel.hpp"

// int main(int argc, char **argv) {
//     // Create users
//     // User alice("Alice");
//     // User bob("Bob");
//     // User charlie("Charlie");

//     // Create channel manager and a channel
//     Channelmanager manager;
//     Channel* general = manager.CreatChannel("#general");

//     // Add users to the channel
//     general->addMember("Alice");
//     general->addMember("Bob");
//     general->addMember("Charlie");

//     // Make Alice an operator
//     general->addOperator("alice");

//     // ✅ Test: Alice kicks Bob
//     std::cout << "\n✅ Alice kicks Bob:\n";
//     // std::vector<std::string> kickCmd1 = {"KICK", "#general", "Bob"};
//     exexc_kick_cmd(split_output(argv[1]), manager);

// 	general->print_members();
//     // // ❌ Test: Bob tries to kick Charlie (Bob is not operator)
//     // std::cout << "\n❌ Bob tries to kick Charlie:\n";
//     // std::vector<std::string> kickCmd2 = {"KICK", "#general", "Charlie"};
//     // exexc_kick_cmd(kickCmd2, manager);

//     // // ❌ Test: Kicking a non-existent user
//     // std::cout << "\n❌ Kick non-existent user:\n";
//     // std::vector<std::string> kickCmd3 = {"KICK", "#general", "Dave"};
//     // exexc_kick_cmd(kickCmd3, manager);

//     // // ❌ Test: Kicking from a non-existent channel
//     // std::cout << "\n❌ Kick from non-existent channel:\n";
//     // std::vector<std::string> kickCmd4 = {"KICK", "#random", "Charlie"};
//     // exexc_kick_cmd(kickCmd4, manager);

//     return 0;
// }