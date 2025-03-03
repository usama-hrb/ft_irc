/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: souaouri <souaouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 11:51:33 by souaouri          #+#    #+#             */
/*   Updated: 2025/02/26 22:09:05 by souaouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once 

#include <iostream>

class User {
private:
    std::string nickname;

public:
    User(std::string nick);
	void	sendMessage(std::string msg);
    std::string getNick();
};