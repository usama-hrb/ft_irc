#include "parsing.hpp"

void	exec_invite_cmd(std::vector<std::string> BUFFER, Channelmanager &manager)
{
	std::string nickname = BUFFER[1];
	std::string channel = BUFFER[2];

	if (BUFFER.size() < 3)
	{
		std::cout << "461" << " <NICKNAME> " <<  BUFFER[0] << " :Not enough parameters" << std::endl;
		return ;
	}
	if (!manager.search_for_channel(channel))
	{
		std::cout << "403" << " <NICKNAME> " << channel << " :No such channel" << std::endl;
		return ;
	}
	if (!manager.search_for_user(nickname))
	{
		std::cout << "401" << " <NICKNAME> " << nickname << " :No such nick" << std::endl;
		return ;		
	}
}
