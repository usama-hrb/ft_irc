
#include "parsing.hpp"
#include "Channel.hpp"

int	parce_channel(std::string channel)
{
	if (channel[0] != '#')
	{
		std::cout << "403" << " <NICKNAME> " << channel << " :No such channel" << std::endl;
		return 0;
	}
	return 1;
}

void	exec_join_cmd(std::vector<std::string> BUFFER, Channelmanager &manager)
{
	// Channelmanager manager;

	std::string		channel_name = BUFFER[1];
	if (BUFFER.size() <= 1)
	{
		std::cout << "461" << " <NICKNAME> " <<  BUFFER[0] << " :Not enough parameters" << std::endl; //:Not enough parameters
		return ;
	}
	if (!parce_channel(channel_name))
		return ;
	manager.CreatChannel(channel_name);
}


// std::vector<std::string>	parse(std::string BUFFER)
// {
// 	if (BUFFER.empty())
// 		exit (1);
// 	std::istringstream iss(BUFFER);
// 	std::string			word;
// 	std::vector<std::string>		words;
// 	while (iss >> word)
// 		words.push_back(word);
// 	return (words);
// 	// for (int i = 0; i < words.size(); ++i)
// 	// {
// 	// 	std::cout << words[i] << std::endl;
// 	// }
// }

// int main (int argc, char **argv)
// {
// 	// split_output(std::string(argv[i]));
//check_is_cmd_valid(split_output(std::string(argv[1])));
	
// }

// int main(int argc, char** argv) {
//     Channelmanager manager;
// 	// Main m;

// 	(void)argc;
// 	// m.run();
//     // Create channels
// 	manager.CreatChannel("#general");
// 	manager.CreatChannel("#random");
// 	manager.CreatChannel("#nonexistent");

// 	check_is_cmd_valid(split_output(argv[1]), manager);

// 	manager.print_channels(); 
//     // Search for channels
//     // Channel *found = manager.search_for_channel("#general");
//     // if (found) {
//     //     std::cout << "Found channel: " << found->getName() << std::endl;
//     // } else {
//     //     std::cout << "Channel not found!" << std::endl;
//     // }

//     // Channel *notFound = manager.search_for_channel("#nonexistent");
//     // std::cout << (notFound ? "Channel found!" : "Channel not found!") << std::endl;

//     return 0;
// }