#pragma once

#include "Needs.hpp"
#include "Client.hpp"
#include "User.hpp"
#include "ReplyCodes.hpp"
#include "Channel.hpp"
// #include ""

class Server {
	public:
		Server(int port, const std::string &password);
		~Server();
		void run();

	private:
	// Server Config
		int 				_port;
		std::string 		_password;
		int 				_serverFd;
		struct sockaddr_in 	_serverAddress;

	// Connect management
		std::vector<pollfd> 			_pollFds;
		std::map<int, Client*> 			_clients;
		ChannelManager 					_channelManager;
		bool 							_running;
	
	//Setup methods
		void createSock();
		void setupAddr();
		void bindSock();
		void startListening();

	// Event handling
		void handleNewConnection();
	    void handleClientData(int fd);
	    void handleClientDisconnect(int fd);
		void closeClient(int clientFd);
	
	// Command processing
		void processCommand(Client* client, const std::string& command);
	    void authenticateClient(Client* client);
		void welcomingMessage(Client* client) const;
		bool isValidNickname(const std::string& nickname);

	// Utility funcs
		bool validatePassword(const std::string& input) const;
	    void sendReplay(int fd, const std::string& response) const;

	// Command hemdlers
		void handlePass(Client* client, const std::vector<std::string>& params);
		void handleNick(Client* client, const std::vector<std::string>& params);
	    void handleUser(Client* client, const std::vector<std::string>& params);
		// void handleJoin(Client* client, const std::vector<std::string>& params);
		void handleList(Client* client, const std::vector<std::string>& params);
	    // void handlePrivmsg(Client* client, const std::vector<std::string>& params);
	    // void handleMode(Client* client, const std::vector<std::string>& params);
	    void handleKick(Client* client, const std::vector<std::string>& params);
	    // void handleInvite(Client* client, const std::vector<std::string>& params);
	    // void handleTopic(Client* client, const std::vector<std::string>& params);
	    // void handleQuit(Client* client, const std::vector<std::string>& params);

	// Prevent copying
		Server(const Server&);
		Server& operator=(const Server&);


	void	exec_join_cmd(std::vector<std::string> BUFFER, Server &manager, Client *c);
	void handleJoin(Client* client, const std::vector<std::string>& params);





	///


	///
	// public:

	// 	Channel*	search_for_channel(std::string channel_name);
	// 	Client* 	search_for_user(const std::string nickname);
	// 	Channel*	CreatChannel(std::string channel_name);
	// 	void		print_channels(){
	// 		for (int i = 0; (size_t)i < _channels.size(); i++)
	// 		{
	// 			std::cout << "--->  channel  :   " << _channels[i]->getName() << std::endl;
	// 		}
	// 	};
};