#include "parsing.hpp"

void	exec_privmsg_cmd(std::vector<std::string> BUFFER, Channelmanager& manager)
{
	std::string channel_name = BUFFER[1];
	std::string nickname = BUFFER[1];
	std::string msg = BUFFER[2];

	if (BUFFER.size() < 2)
	{
		std::cout << "411" << " <NICKNAME> " <<  " :No recipient given" << std::endl;
		return ;
	}
	if (BUFFER.size() < 3)
	{
		std::cout << "412" << " <NICKNAME> " <<  " :No text to send" << std::endl;
		return ;
	}
	if (BUFFER[1][0] == '#')
	{
		// check is my user in the channel
		Channel* channel = manager.search_for_channel(channel_name);
		if (!channel)
		{
			std::cout << "403" << " <NICKNAME> " << channel_name << " :No such channel" << std::endl;
			return ;
		}
		//channel->broadcast("<NIKENAME>" + " PRIVMSG " + " " + channel_name + " :" + msg, "<NICKNAME")
	}
	else if (BUFFER[1][0] != '#')
	{
		User* receiver = manager.search_for_user(nickname);
		
		if (!receiver)
		{
			std::cout << "401" << " <NICKNAME> " << nickname << " :No such nick" << std::endl;
			return ;
		}
		//receiver->sendMessage("<NAKENAME>" + " PRIVMSG " + nickname + " :" + msg);
	}
}

int main(int argc, char **argv) {
    // Create users
    // User alice("Alice");
    // User bob("Bob");
    // User charlie("Charlie");

    // Create channel manager and a channel
    Channelmanager manager;
    Channel* general = manager.CreatChannel("#general");

    // Add users to the channel
    general->addMember("Alice");
    general->addMember("Bob");
    general->addMember("Charlie");

    // Make Alice an operator
    general->addOperator("alice");

    // ✅ Test: Alice kicks Bob
    std::cout << "\n✅ Alice kicks Bob:\n";
    // std::vector<std::string> kickCmd1 = {"KICK", "#general", "Bob"};
    exec_privmsg_cmd(split_output(argv[1]), manager);

	general->print_members();
    // // ❌ Test: Bob tries to kick Charlie (Bob is not operator)
    // std::cout << "\n❌ Bob tries to kick Charlie:\n";
    // std::vector<std::string> kickCmd2 = {"KICK", "#general", "Charlie"};
    // exexc_kick_cmd(kickCmd2, manager);

    // // ❌ Test: Kicking a non-existent user
    // std::cout << "\n❌ Kick non-existent user:\n";
    // std::vector<std::string> kickCmd3 = {"KICK", "#general", "Dave"};
    // exexc_kick_cmd(kickCmd3, manager);

    // // ❌ Test: Kicking from a non-existent channel
    // std::cout << "\n❌ Kick from non-existent channel:\n";
    // std::vector<std::string> kickCmd4 = {"KICK", "#random", "Charlie"};
    // exexc_kick_cmd(kickCmd4, manager);

    return 0;
}