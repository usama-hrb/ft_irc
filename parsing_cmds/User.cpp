/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: souaouri <souaouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 11:55:24 by souaouri          #+#    #+#             */
/*   Updated: 2025/02/26 22:08:47 by souaouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

User::User(std::string nick)
{
	nickname = nick; 
}

void	User::sendMessage(std::string msg)
{
	std::cout << msg << std::endl;
}

std::string User::getNick()
{
	return (nickname);
}