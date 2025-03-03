/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: souaouri <souaouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 12:14:13 by souaouri          #+#    #+#             */
/*   Updated: 2025/03/03 21:46:11 by souaouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.hpp"
#include "Channel.hpp"
#include <sys/socket.h>

void ft_send(int fd, std::string msg) {
	if (send(fd, msg.c_str(), msg.length(), 0) == -1) {
		std::cerr << "send() failed" << std::endl;
	}
}

std::vector<std::string>	split_output(std::string BUFFER)
{
	if (BUFFER.empty())
		exit (1);
	std::istringstream iss(BUFFER);
	std::string			word;
	std::vector<std::string>		words;
	while (iss >> word)
		words.push_back(word);
	return (words);
	// for (int i = 0; i < words.size(); ++i)
	// {
	// 	std::cout << words[i] << std::endl;
	// }
}

void	check_is_cmd_valid(std::vector<std::string> BUFFER, Channelmanager &manager)
{
	int 	i = 0;

	//std::cout << "=============>  :   " << BUFFER[0] << std::endl;
	if (BUFFER[i] == "JOIN")
	{
		exec_join_cmd(BUFFER, manager);
	
	}
	else if (BUFFER[i] == "KICK")
	{
		exec_kick_cmd(BUFFER, manager);
	
	}
	else if (BUFFER[i] == "PRIVMSG")
	{
		exec_privmsg_cmd(BUFFER, manager);
	
	}
	else if (BUFFER[i] == "INVITE")
	{
		exec_invite_cmd(BUFFER, manager);
	
	}
	else
	{
		std::string nuck = "nick";
		ft_send(1, ERR_UNKNOWNCOMMAND(nuck, "ajsd"));
	}
}

// PRIVMSG <nickname> Hello
// PRIVMSG <nickname> :Hello           World

// PRIVMSG
// <nickname>
// :Hello
// World

/*
buffer[1024]


join #dgfdhfdfg  

*/