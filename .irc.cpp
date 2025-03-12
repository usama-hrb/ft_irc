#include <iostream>
#include <vector> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <fcntl.h> 
#include <unistd.h> 
#include <arpa/inet.h> 
#include <poll.h> 
#include <csignal> 
#include <map>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <cerrno>


#define RED "\e[1;31m"
#define WHI "\e[0;37m"
#define GRE "\e[1;32m"
#define YEL "\e[1;33m"
#define END "\e[0;0m"
#define BLU "\e[1;34m"   
#define MAG "\e[1;35m" 
#define CYA "\e[1;36m" 
#define BLA "\e[0;30m" 
#define GRA "\e[1;90m" 
#define PUR "\e[0;95m" 
#define ORA "\e[1;91m"

volatile sig_atomic_t g_keepRunning = 1;

#define PREFIX ":FT_irc "
#define POSTFIX "\r\n"

// Existing Codes
#define RPL_WELCOME(nickname, username, ipaddress) PREFIX " 001 " + nickname + " :Welcome to the Internet Relay Network (" + nickname + "!" + username + "@" + ipaddress + ")" + POSTFIX
#define RPL_YOURHOST(nickname) PREFIX " 002 " + nickname + " :Your host is FT_irc, running version 1.0" + POSTFIX
#define RPL_CREATED(nickname) PREFIX " 003 " + nickname + " :This server was created " + __DATE__ + POSTFIX
#define RPL_MYINFO(nickname) PREFIX " 004 " + nickname + " :FT_irc 1.0 - -" + POSTFIX
#define ERR_NEEDMOREPARAMS(command) PREFIX " 461 " + command + " :Not enough parameters" + POSTFIX
#define ERR_ALREADYREGISTRED(nickname) PREFIX " 462 " + nickname + " :Unauthorized command (already registered)" + POSTFIX
#define ERR_PASSWDMISMATCH(nickname) PREFIX " 464 " + nickname + " :Password incorrect" + POSTFIX
#define ERR_NONICKNAMEGIVEN(nickname) PREFIX " 431 " + nickname + " :No nickname given" + POSTFIX
#define ERR_ERRONEUSNICKNAME(nickname, nick) PREFIX " 432 " + nickname + " " + nick + " :Erroneus nickname" + POSTFIX
#define ERR_NICKNAMEINUSE(nickname, nick) PREFIX " 433 " + nickname + " " + nick + " :Nickname is already in use" + POSTFIX
#define ERR_UNKNOWNCOMMAND(command) PREFIX " 421 " + command + " :Unknown command" + POSTFIX
#define ERR_USERONCHANNEL(nickName ,invitedUser, channelName)  PREFIX " 443 " + nickName + invitedUser + channelName + " :User is already on channel" + POSTFIX
#define ERR_INVITEONLYCHAN(nick, channel) PREFIX " 473 " + nick + " " + channel + " :Cannot join channel (+i)" POSTFIX
#define ERR_CHANNELISFULL(nick, channelName) PREFIX " 471 " + nick + " " + channelName + " :Cannot join channel (+l) - channel is full" + POSTFIX
#define ERR_NOCHANMODES(nickname, channel) PREFIX " 477 " + nickname + " " + channel + " :Channel doesn't support modes" + POSTFIX
#define ERR_BADCHANNELKEY(nick, channelName) PREFIX " 475 " + nick + " " + channelName + " :Cannot join channel (+k) - bad key" + POSTFIX
#define ERR_INVALIDMODEPARAM(nickname, channel, param) PREFIX " 696 " + nickname + " " + channel + " " + param + " :Invalid mode parameter" + POSTFIX
#define RPL_CHANNELMODEIS(nickname, channel, mode, params) PREFIX " 324 " + nickname + " " + channel + " " + mode + " " + params + POSTFIX
#define RPL_CHANGEMODE(chName, mode, arguments ) PREFIX " MODE " + chName + " " + mode + " " + arguments + POSTFIX

// New Codes for Channels
#define RPL_TOPIC(topic, nick, channelName) PREFIX " 332 " + nick + " " + channelName + " :" + topic + POSTFIX
#define RPL_NOTOPIC(nickname, channel) PREFIX " 331 " + nickname + " " + channel + " :No topic is set" + POSTFIX
#define RPL_NAMREPLY(nickname, channel, names) PREFIX " 353 " + nickname + " = " + channel + " :" + names + POSTFIX
#define RPL_ENDOFNAMES(nickname, channel) PREFIX " 366 " + nickname + " " + channel + " :End of /NAMES list" + POSTFIX
#define ERR_NOTREGISTERED(none) PREFIX " 451 " + none + " :You have not registered" + POSTFIX
#define ERR_NOSUCHCHANNEL(nickname, channel) PREFIX " 403 " + nickname + " " + channel + " :No such channel" + POSTFIX
#define ERR_NOTONCHANNEL(channel) PREFIX " 442 " + channel + " :You're not on that channel" + POSTFIX
#define ERR_NOSUCHNICK(nickname) PREFIX " 401 " + nickname + " :No such nick/channel" + POSTFIX
#define RPL_LISTEND(none) PREFIX " 323 " + none + " :END of list" + POSTFIX
#define ERR_CHANOPRIVSNEEDED(channel) PREFIX " 482 " + channel + " :You're not channel operator" + POSTFIX
#define RPL_JOIN(nick, username, channelname, ipaddress) ":" + nick + "!" + username + "@" + ipaddress + " JOIN " + channelname + POSTFIX
#define RPL_INVITING(inviting, invited, channel) PREFIX " 341 " + inviting + " " + invited + " " + channel + POSTFIX
#define RPL_INVITED(nick, invited, channel, info)   ":" + info + " INVITE " + invited + " :" + channel + POSTFIX
#define RPL_QUIT(nickname, message) ":" + nickname + " QUIT : " + message + "\r\n"
#define RPL_OPERATOR(nickname) ":" + nickname + " IS OPERATOR" "\r\n"

//BOT
#define _NOTICE(nickname, message) PREFIX " NOTICE " + nickname + " :" + message + POSTFIX


#define SERV() "\n\
░██████╗███████╗██████╗░██╗░░░██╗███████╗██████╗░\n\
██╔════╝██╔════╝██╔══██╗██║░░░██║██╔════╝██╔══██╗\n\
╚█████╗░█████╗░░██████╔╝╚██╗░██╔╝█████╗░░██████╔╝\n\
░╚═══██╗██╔══╝░░██╔══██╗░╚████╔╝░██╔══╝░░██╔══██╗\n\
██████╔╝███████╗██║░░██║░░╚██╔╝░░███████╗██║░░██║\n\
╚═════╝░╚══════╝╚═╝░░╚═╝░░░╚═╝░░░╚══════╝╚═╝░░╚═╝\n"

class Client {
	int 		_fd;
	std::string _nickName;
	std::string _userName;
	std::string _realName;
	std::string _clientIp;
	std::string _inputBuffer;

	bool _hasPassword;
	bool _hasNickName;
	bool _hasUserName;
	bool _isRegistred;

public:
	Client(int fd);
	~Client();

	// GETTERS
	int getFd() const;
	std::string getClientIp() const;
	std::string getNickName() const;
	std::string getUserName() const;
	std::string getRealName() const;
	bool isRegistred() const;
	bool hasPassword() const;
	bool hasNickName() const;
	bool hasUserName() const;
	std::string getClientInfo() const;

	// SETTERS
	void setNickName(const std::string& nickName);
    void setUserName(const std::string& userName);
    void setRealName(const std::string& realName);
    void setPassword(bool status);
    void setIsRegistered(bool status);

	void appendToBuffer(const std::string& data);
    std::string& getBuffer();
    void clearBuffer(size_t pos);
};

class Channel {
private:
    std::string name;
    std::string password; //changed
    std::vector<Client*> members;
    std::vector<Client*> operators;
	std::vector<char> 		modes;
    std::string _topic;
	// modes
	int inviteOnly;
	int  limit;

public:
	std::vector<Client*> 			_invitedClients;
	bool isInviteOnly(std::string nickName);
	bool checkEmptyOp();
	void setFirtOp();
	void setModes(char mode);
	std::vector<char> getModes();
	void removeMode(char mode);
	void setInviteOnly(int flag);
	int getInviteOnly();
	void setLimit(int limit);
	int  getLimit();
    void setTopic(const std::string &newTopic);
	void	addInvitedOnly(Client* invitedClient);
    std::string getTopic() const;
    size_t getMemrbersNum();
    Channel(std::string new_name);
    std::string getName();
	std::string getPassword();//changed
	void setPassword(std::string password);//changed
    void addMember(Client* client);
    void addOperator(Client* client);
    bool isOperator(Client* client);
    bool isMember(Client* client);
    bool checkForClient(std::string client_name);
	bool isEmpty();
    bool removeMember(std::string nickname, std::string msg);
    void broadcast(const std::string &msg, std::string senderNick);
	void modeBroadcast(const std::string &msg);
    std::vector<std::string> getMemberNames();
    void removeClient(Client* client);
	Client* searchForMember(std::string nickname);
	void addOp(std::string newOp);
	void removeOp(std::string newOp);
};

class ChannelManager {
public:
	~ChannelManager();
    std::vector<Channel*> Channels;
    Channel*	search_for_channel(std::string channel_name);
    Channel*	CreatChannel(std::string channel_name);
	void		removeChannel(const std::string& channelName);
};

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
		Client* findClientByNickname(const std::string& nickname);

	// Searsh funcs
		Client* searchForUser(std::string nickname);

	// 


	// Command handlers
		void handlePass(Client* client, const std::vector<std::string>& params);
		void handleNick(Client* client, const std::vector<std::string>& params);
	    void handleUser(Client* client, const std::vector<std::string>& params);
		void handleJoin(Client* client, const std::vector<std::string>& params);
	    void handlePrivmsg(Client* client, const std::vector<std::string>& params);
	    void handleTopic(Client* client, const std::vector<std::string>& params);
	    void handleKick(Client* client, const std::vector<std::string>& params);
	    void handleInvite(Client* client, const std::vector<std::string>& params);
	    void handleQuit(Client* client, const std::vector<std::string>& params);
	    void handleMode(Client* client, const std::vector<std::string>& params);


	//bot 
		struct Challenge {
			std::string challenger;
			std::string target;
			std::string challenger_choice;
			std::string target_choice;
			bool accepted;
			time_t created_at;
			std::string channel;
		};

		std::map<std::string, Challenge> activeChallenges;
		void handleRPS(Client* client, const std::vector<std::string>& params);
		void handleAccept(Client* client);
		void handleChoose(Client* client, const std::vector<std::string>& params);
		void resolveRPS(const Challenge& challenge);
		std::string replaceAll(std::string str, const std::string& from, const std::string& to);
		void checkChallengeCompletion(Challenge& challenge);
		void checkChallengeTimeouts();
};


void Server::checkChallengeCompletion(Challenge& challenge) {
    if (challenge.challenger_choice.empty() || challenge.target_choice.empty()) {
        return;
    }

    Channel* channel = _channelManager.search_for_channel(challenge.channel);
    if (!channel) return;

    std::string result;
    if (challenge.challenger_choice == challenge.target_choice) {
        result = "Draw! Both chose " + challenge.challenger_choice;
    } else if (
        (challenge.challenger_choice == "rock" && challenge.target_choice == "scissors") ||
        (challenge.challenger_choice == "paper" && challenge.target_choice == "rock") ||
        (challenge.challenger_choice == "scissors" && challenge.target_choice == "paper")
    ) {
        result = challenge.challenger + " wins! " + challenge.challenger_choice + 
                " beats " + challenge.target_choice;
    } else {
        result = challenge.target + " wins! " + challenge.target_choice + 
                " beats " + challenge.challenger_choice;
    }

    channel->broadcast("RPS Result: " + result, "");
    activeChallenges.erase(challenge.challenger);
}

void Server::checkChallengeTimeouts() {
    time_t now = time(NULL);
    std::vector<std::string> to_remove;

    std::map<std::string, Challenge>::iterator it = activeChallenges.begin();
    for (; it != activeChallenges.end(); ++it) {
        const std::string& challenger = it->first;
        const Challenge& challenge = it->second;

        if (now - challenge.created_at > 120) {
            to_remove.push_back(challenger);
            Channel* channel = _channelManager.search_for_channel(challenge.channel);
            if (channel)
                channel->broadcast(_NOTICE(channel->getName(), "RPS challenge from " + challenger + " timed out"), "");
        }
    }

    std::vector<std::string>::iterator vIt = to_remove.begin();
    for (; vIt != to_remove.end(); vIt++) {
        activeChallenges.erase(*vIt);
    }
}

void Server::handleChoose(Client* client, const std::vector<std::string>& params) {
    checkChallengeTimeouts();
    if (params.size() < 2) {
        sendReplay(client->getFd(), _NOTICE(client->getNickName(), "Syntax: /choose <rock|paper|scissors> <channel>"));
        return;
    }
    
    std::string choice = params[0];
    std::string channelName = params[1];
    std::transform(choice.begin(), choice.end(), choice.begin(), ::tolower);

    if (choice != "rock" && choice != "paper" && choice != "scissors") {
        sendReplay(client->getFd(), _NOTICE(client->getNickName(), "Invalid choice. Valid options: rock, paper, scissors"));
        return;
    }

    bool found = false;
    std::map<std::string, Challenge>::iterator it = activeChallenges.begin();
    for (; it != activeChallenges.end(); it++) {
        if (it->second.channel != channelName) continue;
        if (client->getNickName() == it->second.challenger) {
            it->second.challenger_choice = choice;
            found = true;
			if (it->second.target_choice.empty()) {
                Client* target = findClientByNickname(it->second.target);
                if (target)
                    sendReplay(target->getFd(), _NOTICE(target->getNickName(), "[RPS] " + it->second.challenger + " has chosen. Your turn!"));
            }
        } else if (client->getNickName() == it->second.target) {
            if (!it->second.accepted) {
                sendReplay(client->getFd(), _NOTICE(client->getNickName(), "You must accept the challenge before choosing"));
                return;
            }
            it->second.target_choice = choice;
            found = true;
			if (it->second.challenger_choice.empty()) {

			Client* challenger = findClientByNickname(it->second.challenger);
		    if (challenger)
                sendReplay(challenger->getFd(), _NOTICE(challenger->getNickName(), it->second.target + " has chosen. Your turn!"));
			}
        }
        if (found) {
            checkChallengeCompletion(it->second);
            break;
        }
    }
    if (!found)
        sendReplay(client->getFd(), _NOTICE(client->getNickName(), "No active RPS challenge in " + channelName));
}

void Server::handleAccept(Client* client) {
    checkChallengeTimeouts();
    std::map<std::string, Challenge>::iterator it = activeChallenges.begin();
    for (; it != activeChallenges.end(); ++it) {
        if (it->second.target == client->getNickName()) {
            it->second.accepted = true;

            Channel * channel = _channelManager.search_for_channel(it->second.channel);
            if (channel)
                channel->broadcast(_NOTICE(channel->getName(), client->getNickName() + " accepted the RPS challenge!"), "");
            return;
        }
    }
    sendReplay(client->getFd(), _NOTICE(client->getNickName(), "No pending challenges to accept"));
}

void Server::handleRPS(Client* client, const std::vector<std::string>& params) {
    if (params.size() < 2) {
        sendReplay(client->getFd(), _NOTICE(client->getNickName(), " :Syntax: /rps <nickname> <channel>\r\n"));
        return;
    }

    std::string targetNick = params[0];
    std::string channelName = params[1];
    
    if (client->getNickName() == targetNick) {
        sendReplay(client->getFd(), _NOTICE(client->getNickName(), "You can't challenge yourself"));
        return;
    }
    Client* target = findClientByNickname(targetNick);
    if (!target) {
        sendReplay(client->getFd(), _NOTICE(client->getNickName(), "User " + targetNick + " not found"));
        return;
    }
    Channel* channel = _channelManager.search_for_channel(channelName);
    if (!channel) {
        sendReplay(client->getFd(), _NOTICE(client->getNickName(), "Channel " + channelName + " doesn't exist"));
        return;
    }

    Challenge newChallenge;
    newChallenge.challenger = client->getNickName();
    newChallenge.target = targetNick;
    newChallenge.challenger_choice = "";
    newChallenge.target_choice = "";
    newChallenge.accepted = false;
    newChallenge.created_at = time(NULL);
    newChallenge.channel = channelName;
        
    activeChallenges[client->getNickName()] = newChallenge;

    sendReplay(client->getFd(), _NOTICE(client->getNickName(), "Challenge sent to " + targetNick + " in " + channelName));
    sendReplay(target->getFd(), _NOTICE(target->getNickName(), "You've been challenged to RPS by " + client->getNickName() + " in " + channelName + ". Use /accept to confirm"));
    channel->broadcast(_NOTICE(channel->getName(), client->getNickName() + " challenged " + targetNick + " to RPS!"), "");
}


void	Server::handleInvite(Client* client, const std::vector<std::string>& params)
{
	std::string nickName = params[0];
	std::string channelName = params[1];
	std::string reason = "";


	if (params.size() > 2 && params[2][0] == ':')
		reason = params[2];

	if (!client->isRegistred())
	{
        sendReplay(client->getFd(), ERR_NOTREGISTERED(std::string("*")));
        return;
    }
	if (params.empty() || params.size() < 2)
	{
		sendReplay(client->getFd(), ERR_NEEDMOREPARAMS(std::string("INVITE")));
		return ;
	}
	Client* invitedClient = searchForUser(nickName);

	if (!invitedClient)
	{
		sendReplay(client->getFd(), ERR_NOSUCHNICK(client->getNickName()));
		return ;
	}
	Channel* channel = _channelManager.search_for_channel(channelName);
	if (!channel)
	{
		sendReplay(client->getFd(), ERR_NOSUCHCHANNEL(client->getNickName(), channelName));
		return ;
	}
	if (!channel->isOperator(client))
    {
        sendReplay(client->getFd(), ERR_CHANOPRIVSNEEDED(channelName));
        return;
    }
	if (channel->isMember(invitedClient))
	{
		sendReplay(client->getFd(), ERR_USERONCHANNEL(client->getNickName(), invitedClient->getNickName(), channelName));
		return ;
	}

	sendReplay(client->getFd(), RPL_INVITING(client->getNickName(), nickName, channelName));
	sendReplay(invitedClient->getFd(), RPL_INVITED(client->getNickName(), nickName, channelName, client->getClientInfo()));

	channel->addInvitedOnly(invitedClient);
}


std::vector<std::string> split(const std::string& str, char delimiter)
{
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end;

    while ((end = str.find(delimiter, start)) != std::string::npos)
	{
        tokens.push_back(str.substr(start, end - start));
        start = end + 1;
	}
    tokens.push_back(str.substr(start));

    return tokens;
}

void Server::handleJoin(Client* client, const std::vector<std::string>& params) {
    if (!client->isRegistred()) {
        sendReplay(client->getFd(), ERR_NOTREGISTERED(std::string("*")));
        return;
    }
    if (params.empty()) {
        sendReplay(client->getFd(), ERR_NEEDMOREPARAMS(std::string("JOIN")));
        return;
    }
	std::vector<std::string> channels = split(params[0], ',');
	std::vector<std::string> passwords;
	if (params.size() > 1) {
	    std::string passParam = params[1];
	    if (!passParam.empty() && passParam[0] == ':') {
	        passParam = passParam.substr(1);
	    }

	    std::istringstream iss(passParam);
	    std::string token;
	    while (std::getline(iss, token, ' ')) {
	        if (!token.empty()) {
	            std::istringstream tokenStream(token);
	            std::string subToken;
	            while (std::getline(tokenStream, subToken, ',')) {
	                if (!subToken.empty())
	                    passwords.push_back(subToken);
	            }
	        }
	    }
	}
	for(size_t i = 0; i < channels.size(); i++)
	{
		if (channels[i] == "#") {
	        sendReplay(client->getFd(), ERR_NEEDMOREPARAMS(std::string("JOIN")));
	        return;
		}
		std::string channelName = channels[i];
		if (channelName[0] != '#') {
			sendReplay(client->getFd(), ERR_NOSUCHCHANNEL(client->getNickName(), channelName));
			continue;
		}
		Channel* channel = _channelManager.search_for_channel(channelName);
		std::string password = (i < passwords.size()) ? passwords[i] : "";
		if (channel && !channel->getPassword().empty())
		{
			if (channel->getPassword() != password)
			{
				sendReplay(client->getFd(), ERR_BADCHANNELKEY(client->getNickName(), channelName));
				continue;
			}
		}
		if (channel && channel->getLimit() != 0)
		{
			if (channel->getMemrbersNum() == (size_t)channel->getLimit())
			{
				sendReplay(client->getFd(), ERR_CHANNELISFULL(client->getNickName(),channelName));
				continue;
			}
		}
		if (channel && channel->getInviteOnly() == 1)
		{
			if (!channel->isInviteOnly(client->getNickName()))
			{
				sendReplay(client->getFd(), ERR_INVITEONLYCHAN(client->getNickName(),channelName));
				continue;
			}
		}
		if (channel && channel->isMember(client)) {
            sendReplay(client->getFd(), ERR_USERONCHANNEL(client->getNickName(), client->getNickName(), channelName));
            continue;
        }
		if (!channel)
		{
			channel = _channelManager.CreatChannel(channelName);
			if (!password.empty())
				channel->setPassword(password);
			channel->addOperator(client);
		}
		channel->addMember(client);

		sendReplay(client->getFd(), RPL_JOIN(client->getNickName(), client->getUserName(), channelName, client->getClientIp()));
		channel->broadcast(RPL_JOIN(client->getNickName(), client->getUserName(), channelName, client->getClientIp()), client->getNickName());

		std::vector<std::string> names = channel->getMemberNames();
        std::string namesStr;
        for (size_t j = 0; j < names.size(); j++) {
            if (!namesStr.empty()) namesStr += " ";
            namesStr += names[j];
        }
		std::string topic = channel->getTopic();
		if (topic[0] == ':')
			topic = topic.substr(1);
		sendReplay(client->getFd(), RPL_TOPIC(topic,client->getNickName(), channelName));
        sendReplay(client->getFd(), RPL_NAMREPLY(client->getNickName(), channelName, namesStr));
        sendReplay(client->getFd(), RPL_ENDOFNAMES(client->getNickName(), channelName));
	}
}


std::string clientIp(int client_fd)
{
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    char client_ip[INET_ADDRSTRLEN];

    if (getpeername(client_fd, (struct sockaddr *)&client_addr, &client_len) == -1)
		throw std::runtime_error("getpeername failed");
    if (inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip)) == NULL)
		throw std::runtime_error("inet_ntop failed");
    return std::string(client_ip);
}


void Server::handlePass(Client *client, const std::vector<std::string> &params) {
	std::string response;
	if (client->hasPassword()) {
		std::string msg = ERR_ALREADYREGISTRED(std::string("*"));
		send(client->getFd(),msg.c_str() , msg.size(), 0);
		return;
	}
	if (params.empty()) {
		response = ERR_NEEDMOREPARAMS(std::string("PASS"));
		sendReplay(client->getFd(), response);
		return;
	}
	if (params[0] != _password) {
		response = ERR_PASSWDMISMATCH(std::string("*"));
		sendReplay(client->getFd(), response);
		return;
	}
	client->setPassword(true);
}


void Server::handleNick(Client* client, const std::vector<std::string>& params) {
    if (!client->hasPassword()) {
        sendReplay(client->getFd(), ERR_NOTREGISTERED(std::string("*")));
        return;
    }
    if (params.size() != 1) {
        sendReplay(client->getFd(), ERR_NEEDMOREPARAMS(std::string("NICK")));
        return;
    }
    if (params.empty()) {
        sendReplay(client->getFd(), ERR_NONICKNAMEGIVEN(client->getNickName()));
        return;
    }

    std::string nickname = params[0];

    if (client->getNickName() == nickname) return;

    if (!isValidNickname(nickname)) {
        sendReplay(client->getFd(), ERR_ERRONEUSNICKNAME(client->getNickName(), nickname));
        return;
    }
    if (findClientByNickname(nickname)) {
        sendReplay(client->getFd(), ERR_NICKNAMEINUSE(client->getNickName(), nickname));
        return;
    }

    std::string oldNick = client->getNickName();
    client->setNickName(nickname);

    std::string msg = ":" + oldNick + " NICK :" + nickname + "\r\n";
    
    for (std::vector<Channel*>::iterator it = _channelManager.Channels.begin(); it != _channelManager.Channels.end(); ++it) {
        Channel* channel = *it;
        if (channel->isMember(client))
            channel->broadcast(msg, oldNick);
    }
	authenticateClient(client);
}

bool Server::isValidNickname(const std::string& nickname) {
    if (nickname.empty() || nickname.length() > 9)
        return false;
    const std::string special = "-[]\\`^{}";
    for (size_t i = 0; i < nickname.length(); ++i) {
        char c = nickname[i];
        if (i == 0 && !isalpha(c) && special.find(c) == std::string::npos)
            return false;
        if (!isalnum(c) && special.find(c) == std::string::npos)
            return false;
    }
    return true;
}


void Server::handleMode(Client* client, const std::vector<std::string>& params)
{
	if (!client->isRegistred()) {
        sendReplay(client->getFd(), ERR_NOTREGISTERED(std::string("*")));
        return;
    }
    if (params.empty())
	{
        sendReplay(client->getFd(), ERR_NEEDMOREPARAMS(std::string("MODE")));
        return;
    }

	std::string channelName = params[0];
	Channel *channel = _channelManager.search_for_channel(channelName);
	if (!channel)
	{
		sendReplay(client->getFd(), ERR_NOSUCHCHANNEL(client->getNickName(), channelName));
		return ;
	}
	if (params.size() == 1)
	{
		std::string modes = "";
		for (size_t i = 0; i < channel->getModes().size(); i++)
		{
			modes += channel->getModes()[i];
		}
		std::string modeMsg = ": 324 " + client->getNickName() + " " + channelName + " " + (modes.empty() ? "(no modes set)" : "+" + modes) + "\r\n";
		sendReplay(client->getFd(), modeMsg);


		return;
	}
	if (!channel->isOperator(client))
    {
        sendReplay(client->getFd(), ERR_CHANOPRIVSNEEDED(channelName));
        return;
    }
	if (params[1].size() == 2 && (params[1][0] == '-' || params[1][0] == '+'))
	{
		char sign = params[1][0];
		if (params[1][1] == 'i')
		{
			if (params.size() > 2) {
				sendReplay(client->getFd(), ERR_INVALIDMODEPARAM(client->getNickName(), channelName, params[2]));
				return;
			}
			if (sign == '+')
			{
				channel->setInviteOnly(1);
				channel->setModes('i');
			}
			else
			{
				channel->setInviteOnly(0);
				channel->removeMode('i');
			}
		}
		else if (params[1][1] == 't')
		{
			if (sign == '+')
			{
				if (params.size() < 3) {
					sendReplay(client->getFd(), ERR_NEEDMOREPARAMS(std::string("MODE"))); return;
				}	
				std::vector<std::string> vecTopic(params.begin() + 2, params.end());
			    std::string newTopic = ":";
				for (std::vector<std::string>::iterator it = vecTopic.begin(); it != vecTopic.end(); it++)
					newTopic += *it + " ";
				channel->setTopic(newTopic);
			}
			else
			{
				if (params.size() > 2) {
					sendReplay(client->getFd(), ERR_INVALIDMODEPARAM(client->getNickName(), channelName, params[2])); return;
				}	
				channel->setTopic("No topic is set");
			}
		}
		else if (params[1][1] == 'k')
		{
			if (sign == '+')
			{
				if (params.size() < 3) {
					sendReplay(client->getFd(), ERR_NEEDMOREPARAMS(std::string("MODE"))); return;
				}
				if (params.size() > 3){
					sendReplay(client->getFd(), ERR_INVALIDMODEPARAM(client->getNickName(), channelName, params[2])); return;
				}
				channel->setPassword(params[2]);
				channel->setModes('k');
			}
			else
			{
				if (params.size() > 2){
					sendReplay(client->getFd(), ERR_INVALIDMODEPARAM(client->getNickName(), channelName, params[2])); return;
				}
				channel->setPassword("");
				channel->removeMode('k');
			}
		}
		else if (params[1][1] == 'o')
		{
			if (params.size() < 3)
			{
				sendReplay(client->getFd(), ERR_NEEDMOREPARAMS(std::string("MODE")));
				return;
			}
			if (params.size() > 3)
			{
				sendReplay(client->getFd(), ERR_INVALIDMODEPARAM(client->getNickName(), channelName, params[2]));
				return;
			}
			Client* mem = searchForUser(params[2]);
			if (sign == '+')
			{
				if (!channel->isMember(mem))
				{
					sendReplay(client->getFd(), ERR_INVALIDMODEPARAM(client->getNickName(), channelName, params[2]));
					return ;
				}
				if (channel->isOperator(mem))
				{
					sendReplay(client->getFd(), ERR_INVALIDMODEPARAM(client->getNickName(), channelName, params[2]));
					return ;
				}
				channel->addOp(mem->getNickName());
			}
			else 
			{
				
				if (!channel->isOperator(mem) || params.size() > 3)
				{
					sendReplay(client->getFd(), ERR_INVALIDMODEPARAM(client->getNickName(), channelName, params[2]));
					return ;
				}
				channel->removeOp(mem->getNickName());
			}

		}
		
		else if (params[1][1] == 'l')
		{
			if (sign == '+')
			{
				if (params[2].empty())
				{
					sendReplay(client->getFd(), ERR_NEEDMOREPARAMS(std::string("MODE")));
					return ;
				}
				if (params.size() > 3)
				{
					sendReplay(client->getFd(), ERR_INVALIDMODEPARAM(client->getNickName(), channelName, params[2]));
					return;
				}
				int lim = 0;
				std::stringstream iss(params[2]);

				iss >> lim;
				for (size_t i = 0; i < params[2].size(); i++)
				{
					if (params[2].empty() || params[2][i] < '1' || params[2][i] > '9'){
						sendReplay(client->getFd(), ERR_INVALIDMODEPARAM(client->getNickName(), channelName, params[2]));
						return;
					}
				}
				if (lim < static_cast<int>(channel->getMemrbersNum())) {
					sendReplay(client->getFd(), _NOTICE(client->getNickName(), std::string("Channel mumbers more than limit you set!")));
					return;
				}
				channel->setLimit(lim);
				channel->setModes('l');
			}
			else
			{
				if (params.size() > 2)
				{
					sendReplay(client->getFd(), ERR_INVALIDMODEPARAM(client->getNickName(), channelName, params[2]));
					return;
				}
				channel->setLimit(0);
				channel->removeMode('l');
			}
		}
		else
		{
			sendReplay(client->getFd(), ERR_NOCHANMODES(client->getNickName(), channelName));
			return;
		}
	}
	else if (!params[2].empty() && params[2] != "+sn")
	{
		sendReplay(client->getFd(), ERR_NOCHANMODES(client->getNickName(), channelName));
		return ;
	}
	if (params.size() > 2)
		channel->modeBroadcast(RPL_CHANGEMODE(channelName, params[1], params[2]));
	else
	 	channel->modeBroadcast(RPL_CHANGEMODE(channelName, params[1], ""));
}


void Server::handleKick(Client* client, const std::vector<std::string>& params)
{
	if (!client->isRegistred()) {
        sendReplay(client->getFd(), ERR_NOTREGISTERED(std::string("*")));
        return;
    }
    if (params.size() < 2)
    {
        sendReplay(client->getFd(), ERR_NEEDMOREPARAMS(std::string("KICK")));
        return;
    }
    std::string channelName = params[0];
    std::string nickname = params[1];
	std::string reason = (params[2].empty()) ? "Kicked by operator" : params[2];
    Channel* channel = _channelManager.search_for_channel(channelName);
    if (!channel)
    {
        sendReplay(client->getFd(), ERR_NOSUCHCHANNEL(client->getNickName(), channelName));
        return;
    }
    if (!channel->isOperator(client))
    {
        sendReplay(client->getFd(), ERR_CHANOPRIVSNEEDED(channelName));
        return;
    }

	if (!channel->checkForClient(nickname))
	{
		sendReplay(client->getFd(), ERR_NOSUCHNICK(client->getNickName()));
		return ;
	}
    std::string kickMsg = ":" + client->getNickName() + " KICK " + channelName + " " + nickname + " :" + reason + "\r\n";

    channel->removeMember(nickname, kickMsg);
    channel->broadcast(kickMsg, "");


    std::vector<std::string> names = channel->getMemberNames();
    std::string namesStr;
    for (size_t i = 0; i < names.size(); i++)
    {
        if (!namesStr.empty()) namesStr += " ";
        namesStr += names[i];
    }
    std::string namesReply = RPL_NAMREPLY(client->getNickName(), channelName, namesStr);
    std::string endOfNames = RPL_ENDOFNAMES(client->getNickName(), channelName);

    if (channel->isEmpty())
        _channelManager.removeChannel(channelName);
}

void Server::handleUser(Client *client, const std::vector<std::string> &params) {
	if (!client->hasPassword()) {
        sendReplay(client->getFd(), ERR_NOTREGISTERED(std::string("*")));
        return;
    }
	std::string response;
	if (client->isRegistred()) {
		sendReplay(client->getFd(), ERR_ALREADYREGISTRED(std::string("*")));
		return;
	}
	if (params.size() < 4) {
		sendReplay(client->getFd(), ERR_NEEDMOREPARAMS(std::string("USER")));
		return;
	}
	client->setUserName(params[0]);
	client->setRealName(params[3].substr(1));
	authenticateClient(client);
}

void Server::handleTopic(Client* client, const std::vector<std::string>& params) {
    if (!client->isRegistred()) {
        sendReplay(client->getFd(), ERR_NOTREGISTERED(std::string("*")));
        return;
    }

    if (params.empty()) {
        sendReplay(client->getFd(), ERR_NEEDMOREPARAMS(std::string("TOPIC")));
        return;
    }

    std::string channelName = params[0];
    Channel* channel = _channelManager.search_for_channel(channelName);

    if (!channel) {
        sendReplay(client->getFd(), ERR_NOSUCHCHANNEL(client->getNickName(), channelName));
        return;
    }

    if (!channel->isMember(client)) {
        sendReplay(client->getFd(), ERR_NOTONCHANNEL(channelName));
        return;
    }

    if (params.size() == 1) {
        std::string topic = channel->getTopic();
            sendReplay(client->getFd(), RPL_TOPIC(topic, client->getNickName(), channelName));
    } else {
        if (!channel->isOperator(client)) {
            sendReplay(client->getFd(), ERR_CHANOPRIVSNEEDED(channelName));
            return;
        }

        std::string newTopic = (params[1].empty()) ? "" : params[1];
        channel->setTopic(newTopic);

        if (newTopic[0] == ':')
            newTopic = newTopic.substr(1);
        std::string topicMsg = ":" + client->getNickName() + " TOPIC " + channelName + " :" + newTopic + "\r\n";
        channel->broadcast(topicMsg, "");
    }
}

void Server::handleQuit(Client* client, const std::vector<std::string>& params) {
    std::string quitMessage = "";
    if (!params.empty()) {
        quitMessage = params[0];
    }
    for (std::vector<Channel*>::iterator it = _channelManager.Channels.begin(); it != _channelManager.Channels.end(); ++it) {
        if ((*it)->isMember(client)) {
            (*it)->broadcast(RPL_QUIT(client->getNickName(), quitMessage), quitMessage), client->getNickName();
			if ((*it)->isOperator(client))
				(*it)->setFirtOp();
			
        }
    }
    std::cout << RED << "Client with fd : " << client->getFd() << " disconnected" << END << std::endl;
    closeClient(client->getFd());
}

Client* Server::findClientByNickname(const std::string& nickname) {
    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->second->getNickName() == nickname) {
            return it->second;
        }
    }
    return NULL;
}

void Server::handlePrivmsg(Client* client, const std::vector<std::string>& params) {
    if (!client->isRegistred()) {
        sendReplay(client->getFd(), ERR_NOTREGISTERED(std::string("*")));
        return;
    }
    if (params.size() < 2) {
        sendReplay(client->getFd(), ERR_NEEDMOREPARAMS(std::string("PRIVMSG")));
        return;
    }
    
    std::string target = params[0];
    std::string recvmessage = params[1];

    std::string senderInfo = client->getNickName() + "!" + client->getUserName() + "@" + client->getClientIp();
    std::string msgToSend = ":" + senderInfo + " PRIVMSG " + target + " :" + recvmessage + "\r\n";

    if (target[0] == '#') {
        Channel* channel = _channelManager.search_for_channel(target);
        if (!channel) {
            sendReplay(client->getFd(), ERR_NOSUCHCHANNEL(client->getNickName(), target));
            return;
        }
        if (!channel->isMember(client)) {
            sendReplay(client->getFd(), ERR_NOTONCHANNEL(target));
            return;
        }
        channel->broadcast(msgToSend, client->getNickName());
    } else {
        Client* recipient = findClientByNickname(target);
        if (!recipient) {
            sendReplay(client->getFd(), ERR_NOSUCHNICK(client->getNickName()));
            return;
        }
        send(recipient->getFd(), msgToSend.c_str(), msgToSend.size(), 0);
    }
}

Client::Client(int fd) : _fd(fd), _hasPassword(false), _hasNickName(false), _hasUserName(false), _isRegistred(false) {
	struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    char clientIp[INET_ADDRSTRLEN];

    if (getpeername(fd, (struct sockaddr *)&client_addr, &client_len) == -1)
		throw std::runtime_error("getpeername failed");
    if (inet_ntop(AF_INET, &client_addr.sin_addr, clientIp, sizeof(clientIp)) == NULL)
		throw std::runtime_error("inet_ntop failed");
    _clientIp = std::string(clientIp);
}

std::string Client::getClientInfo() const {
    return _nickName + "!" + _userName + "@" + _clientIp;
}

Client::~Client() {}

void Client::appendToBuffer(const std::string& data) { _inputBuffer += data;}
std::string& Client::getBuffer() { return _inputBuffer; }
void Client::clearBuffer(size_t pos) { _inputBuffer.erase(0, pos);}

// GETTERS
int Client::getFd() const {return _fd;}
std::string Client::getClientIp() const {return  _clientIp;}
std::string Client::getNickName() const {return _nickName;}
std::string Client::getUserName() const {return _userName;}
std::string Client::getRealName() const {return _realName;}
bool Client::isRegistred() const {return _isRegistred;}
bool Client::hasPassword() const {return _hasPassword;}
bool Client::hasNickName() const {return _hasNickName;}
bool Client::hasUserName() const {return _hasUserName;}


// SETTERS
void Client::setNickName(const std::string& nickName) {
	_nickName = nickName, _hasNickName = true;
}
void Client::setUserName(const std::string& userName) {
	_userName = userName, _hasUserName = true;
}
void Client::setRealName(const std::string& realName) {
	_realName = realName;
}
void Client::setPassword(bool status) {
	_hasPassword = status;
}
void Client::setIsRegistered(bool status) {
	_isRegistred = status;
}

Channel::Channel(std::string new_name) : name(new_name), password(""), _topic("No topic is set"), inviteOnly(0), limit(0) {}

ChannelManager::~ChannelManager() {
	for (size_t i = 0; i < Channels.size(); ++i) {
        delete Channels[i];
    }
    Channels.clear();
}

bool Channel::checkEmptyOp() {
	if (operators.size() < 2)
		return true;
	else return false;
}

void Channel::setFirtOp() {
	std::string name;
	if (operators.empty() && !members.empty()) {
		operators.push_back(members[0]);
		name = "@" + operators[0]->getNickName();
		operators[0]->setNickName(name);
	}
}

void Channel::addOp(std::string newOp) {
	std::string name;
	Client* client = searchForMember(newOp);
	name = client->getNickName();
	client->setNickName(name);
	operators.push_back(client);
}


Client* Channel::searchForMember(std::string nickname) {
	for (size_t i = 0; i < members.size(); ++i) {
        if (nickname == members[i]->getNickName()) {
            return members[i];
        }
    }
    return NULL;
}

void Channel::setInviteOnly(int flag){
	inviteOnly = flag;
}

int Channel::getInviteOnly(){return inviteOnly;}

void Channel::setLimit(int limit) {
	this->limit = limit;
}

int Channel::getLimit(){return limit;}

bool Channel::isInviteOnly(std::string nickName)
{
	for(size_t i = 0; i < _invitedClients.size(); i++)
	{
		if (_invitedClients[i]->getNickName() == nickName)
			return true;
	}
	return false;
}

void	Channel::addInvitedOnly(Client* invitedClient)
{
	for (size_t i = 0; i < _invitedClients.size(); i++)
	{
		if (_invitedClients[i] == invitedClient)
			return ;
	}
	_invitedClients.push_back(invitedClient);
}

void Channel::removeClient(Client* client) {
    bool wasOp = isOperator(client);

    members.erase(std::remove(members.begin(), members.end(), client), members.end());
    operators.erase(std::remove(operators.begin(), operators.end(), client), operators.end());

    if (wasOp && operators.empty() && !members.empty()) {
        Client* newOp = members.front();
        addOperator(newOp);

        std::string modeMsg = ":FT_irc MODE " + getName() + " +o " + newOp->getNickName() + "\r\n";
        broadcast(modeMsg, "");
    }
}
size_t Channel::getMemrbersNum() {return members.size();}

void Channel::setTopic(const std::string &newTopic) {
       _topic = newTopic;
}


std::string Channel::getTopic() const {
    return _topic;
}

std::string Channel::getName() {
    return name;
}

std::string Channel::getPassword() {return password;}

void Channel::setPassword(std::string pass) {
    password = pass;
}

void Channel::setModes(char mode)
{
	modes.push_back(mode);
}

std::vector<char> Channel::getModes()
{
	return modes;
}

void Channel::removeMode(char mode)
{
	for (std::vector<char>::iterator it = modes.begin(); it != modes.end(); ++it) {
    	if ((*it) == mode) {
            modes.erase(it); return;
		}
    }
}

void Channel::addMember(Client* client) {
    if (std::find(members.begin(), members.end(), client) == members.end()) {
        members.push_back(client);
    }
}

void Channel::addOperator(Client* client) {
    if (!isOperator(client)) {
        operators.push_back(client);
    }
}

bool Channel::isOperator(Client* client) {
    return std::find(operators.begin(), operators.end(), client) != operators.end();
}

bool Channel::isMember(Client* client) {
    return std::find(members.begin(), members.end(), client) != members.end();
}

bool Channel::checkForClient(std::string client_name)
{
    for (size_t i = 0; i < members.size(); ++i) {
        if (client_name == members[i]->getNickName())
            return true;
    }
    return false;
}

bool Channel::isEmpty()
{
	if (!members.size())
		return true;
	return false;
}

bool Channel::removeMember(std::string nickname, std::string msg)
{
    for (std::vector<Client*>::iterator it = members.begin(); it != members.end(); ++it) {
        if ((*it)->getNickName() == nickname)
		{
			send((*it)->getFd(), msg.c_str(), msg.size(), 0);
            members.erase(it);
            return true;
        }
    }
    return false;
}

void Channel::removeOp(std::string nickname)
{
    for (std::vector<Client*>::iterator it = operators.begin(); it != operators.end(); ++it) {
        if ((*it)->getNickName() == nickname) {
            operators.erase(it); return;
		}
    }
}

void Channel::broadcast(const std::string &msg, std::string senderNick){
    for (size_t i = 0; i < members.size(); ++i) {
        if (members[i]->getNickName() != senderNick) {
            send(members[i]->getFd(), msg.c_str(), msg.size(), 0);
        }
    }
}

void Channel::modeBroadcast(const std::string &msg){
    for (size_t i = 0; i < members.size(); ++i) {
        send(members[i]->getFd(), msg.c_str(), msg.size(), 0);
    }
}

std::vector<std::string> Channel::getMemberNames() {
    std::vector<std::string> names;
    for (size_t i = 0; i < members.size(); ++i) {
        std::string name = members[i]->getNickName();
        for (size_t j = 0; j < operators.size(); j++) {
            if (operators[j]->getNickName() == members[i]->getNickName()) {
                name = "@" + name;  // Prepend @ for operators
                break;
            }
        }
        names.push_back(name);
    }
    return names;
}

Channel* ChannelManager::search_for_channel(std::string channel_name) {
    for (size_t i = 0; i < Channels.size(); ++i) {
        if (channel_name == Channels[i]->getName()) {
            return Channels[i];
        }
    }
    return NULL;
}

Channel* ChannelManager::CreatChannel(std::string channel_name) {
    Channel* exist_channel = search_for_channel(channel_name);
    if (!exist_channel) {
        Channel* newChannel = new Channel(channel_name);
        Channels.push_back(newChannel);
        return newChannel;
    }
    return exist_channel;
}

void ChannelManager::removeChannel(const std::string& channelName)
{
    for (std::vector<Channel*>::iterator it = Channels.begin(); it != Channels.end(); ++it) {
        if ((*it)->getName() == channelName) {
            delete *it;
            Channels.erase(it);
            return;
        }
    }
}

void Server::createSock() {
	_serverFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverFd == -1)
		throw (std::runtime_error("socket error : " + std::string(strerror(errno))));
	int opt = 1;
	if (setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
		close(_serverFd), throw (std::runtime_error("setsockopt error : " + std::string(strerror(errno))));
	if (fcntl(_serverFd, F_SETFL, O_NONBLOCK) == -1)
		close(_serverFd), throw (std::runtime_error("fcntl error : " + std::string(strerror(errno))));
}

void Server::setupAddr() {
	memset(&_serverAddress, 0, sizeof(_serverAddress));
	_serverAddress.sin_family = AF_INET;
	_serverAddress.sin_addr.s_addr = INADDR_ANY;
	_serverAddress.sin_port = htons(_port);
}

void Server::bindSock() {
	if (bind(_serverFd, (struct sockaddr *)&_serverAddress, sizeof(_serverAddress)) == -1)
		close(_serverFd), throw (std::runtime_error("bind error : " + std::string(strerror(errno))));
}

void Server::startListening() {
	if (listen(_serverFd, SOMAXCONN) == -1)
		close(_serverFd), throw (std::runtime_error("listen error : " + std::string(strerror(errno))));
	std::cout << GRE << SERV() << END << std::endl;
	std::cout << GRE << "\tServer is running on port " << _port << std::endl;
	std::cout << YEL << "\tWaiting for connections..." << std::endl;
}

void Server::handleNewConnection() {

	struct sockaddr_in 	clientAddress;
	socklen_t			clientLen = sizeof(clientAddress);
	int 				clientFd = accept(_serverFd, (struct sockaddr*)&clientAddress, &clientLen);
	if (clientFd == -1) {
		if (errno == EAGAIN || errno == EWOULDBLOCK) {
			return;
		}
		std::cerr << "accept error : " << strerror(errno) << std::endl;
		return;
	}
	if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1) {
		close(clientFd), std::cerr << "fcntl error when add new client : " << strerror(errno) << std::endl;
		return; 
	}
	Client *newClient = new Client(clientFd);
	_clients[clientFd] = newClient;
	_pollFds.push_back((pollfd){clientFd, POLLIN, 0});
	std::cout << BLU << "New client connected with fd: " << clientFd << END << std::endl;
}

void Server::handleClientData(int clientFd) {
    char buffer[1024];
    ssize_t bytesRead = recv(clientFd, buffer, sizeof(buffer) - 1, 0);

    if (bytesRead <= 0) {
        if (bytesRead == 0) {
			Client* client = _clients[clientFd];
	        if (client) {
	            handleQuit(client, std::vector<std::string>(0));
	        }
            return;
        } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return;
        } 
        else {
			Client* client = _clients[clientFd];
	        if (client) {
	            handleQuit(client, std::vector<std::string>(0));
	        }
            std::cerr << "recv error: " << strerror(errno) << std::endl;
            return;
        }
    }

    buffer[bytesRead] = '\0';
    Client* client = _clients[clientFd];
	client->appendToBuffer(buffer);
	
	std::string buff = client->getBuffer();
	if (buff.substr(0, 4) != "PING" && buff.substr(0, 4) != "PONG")
		std::cout << GRA << "--> " <<  client->getBuffer().substr(0, client->getBuffer().find_last_of('\n')) << END << std::endl;

    size_t pos;
   	while (true) {
        pos = client->getBuffer().find("\r\n");
        if (pos != std::string::npos) {
            std::string message = client->getBuffer().substr(0, pos);
            client->clearBuffer(pos + 2);
            processCommand(client, message);
            continue;
        }

        pos = client->getBuffer().find('\n');
        if (pos != std::string::npos) {
            std::string message = client->getBuffer().substr(0, pos);
            client->clearBuffer(pos + 1);
            processCommand(client, message);
            continue;
        }
        break;
    }
}

void Server::processCommand(Client* client, const std::string& command) {
	std::istringstream iss(command);
	std::vector<std::string> params;
	std::string cmd;
	std::string response;

	iss >> cmd;
	for (std::string param; iss >> param; ) {
		if (param[0] == ':' && param.size() > 1) {
			std::string lastParam = param.substr(1);
			std::string rest;
			std::getline(iss, rest);
			lastParam += rest;
			params.push_back(lastParam);
			break;
		}
		params.push_back(param);
	}
	std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);
	if (cmd == "PASS") handlePass(client, params);
	else if (cmd == "NICK") handleNick(client, params);
	else if (cmd == "USER") handleUser(client, params);
	else if (cmd == "JOIN") handleJoin(client, params);
	else if (cmd == "KICK") handleKick(client, params);
	else if (cmd == "TOPIC") handleTopic(client, params);
	else if (cmd == "INVITE") handleInvite(client, params);
	else if (cmd == "PRIVMSG") handlePrivmsg(client, params);
	else if (cmd == "MODE") handleMode(client, params);
	else if (cmd == "QUIT") handleQuit(client, params);
	else if (cmd == "RPS") handleRPS(client,params);
	else if (cmd == "CHOOSE") handleChoose(client, params);
	else if (cmd == "ACCEPT") handleAccept(client);
	else if (cmd != "PONG" && cmd != "PING") {
		response = ERR_UNKNOWNCOMMAND(cmd);
		sendReplay(client->getFd(), response);
	}
}

void Server::closeClient(int clientFd) {
    std::map<int, Client*>::iterator it = _clients.find(clientFd);
    if (it == _clients.end()) return;

    Client* client = it->second;
    std::vector<std::string> channelsToRemove;
    for (std::vector<Channel*>::iterator chanIt = _channelManager.Channels.begin(); chanIt != _channelManager.Channels.end(); ++chanIt) {
        if ((*chanIt)->isMember(client)) {
            (*chanIt)->removeClient(client);
            if ((*chanIt)->isEmpty()) {
                channelsToRemove.push_back((*chanIt)->getName());
            }
        }
    }
    for (size_t i = 0; i < channelsToRemove.size(); ++i) {
        _channelManager.removeChannel(channelsToRemove[i]);
    }
    for (std::vector<pollfd>::iterator pollIt = _pollFds.begin(); pollIt != _pollFds.end(); ++pollIt) {
        if (pollIt->fd == clientFd) {
            _pollFds.erase(pollIt);
            break;
        }
    }
    delete client;
    _clients.erase(clientFd);

    close(clientFd);
}

Server::Server(int port, const std::string &password) : _port(port), _password(password), _serverFd(-1), _running(false) {
	createSock();
	setupAddr();
	bindSock();
	startListening();
}

Server::~Server() {
    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        close(it->first);
        delete it->second;
    }
    close(_serverFd);
}


void Server::authenticateClient(Client *client) {
	if (client->hasPassword() && client->hasNickName() && client->hasUserName() && !client->isRegistred()) {
		client->setIsRegistered(true);
		welcomingMessage(client);
	}
}

void Server::welcomingMessage(Client* client) const {
	sendReplay(client->getFd(), RPL_WELCOME(client->getNickName(), client->getUserName(), client->getClientIp()));
	sendReplay(client->getFd(), RPL_YOURHOST(client->getNickName()));
	sendReplay(client->getFd(), RPL_CREATED(client->getNickName()));
	sendReplay(client->getFd(), RPL_MYINFO(client->getNickName()));
}

void Server::sendReplay(int fd, const std::string& response) const {
	std::string formatted = response;
	if (formatted.find("\r\n") == std::string::npos)
		formatted += "\r\n";
    send(fd, formatted.c_str(), formatted.size(), 0);
}
void Server::run() {
	_running = true;
	_pollFds.push_back((pollfd){_serverFd, POLLIN, 0});

	while (_running && g_keepRunning) {
		int state = poll(&_pollFds[0], _pollFds.size(), -1);

		if (state < 0 && errno != EINTR)
			throw (std::runtime_error("poll error : " + std::string(strerror(errno))));
		for (size_t i = 0; i < _pollFds.size(); i++) {
			if (_pollFds[i].fd == _serverFd)
				handleNewConnection();
			else
			 	handleClientData(_pollFds[i].fd);
		}
	}
	_running = false;
}

Client* Server::searchForUser(std::string nickname)
{
    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->second && it->second->getNickName() == nickname) {
            return it->second;
        }
    }
    return NULL;
}

void handleShutdown(int signum) {
    (void)signum;
    g_keepRunning = 0;
}

int validPort(std::string sPort) {
    size_t i = 0;
    if (sPort[0] == '+')
        i++;
    for (; i < sPort.size(); i++) {
        if (!std::isdigit(sPort[i]))
            return -1;
    }

    int port;
    std::stringstream ss(sPort);
    ss >> port;

    if (ss.fail() || (port < 1024 || port > 65535))
        return -1;
    return port;
}

int checkPassword(std::string password) {
    if (password.size() > 9 || password.size() < 5)
        return -1;
    for (size_t i = 0; i < password.size(); i++) {
        if (!std::isprint(password[i]) || std::isspace(password[i]))
            return -1;
    }
    return 0;
}

int main(int ac, char** av) {
    if (ac != 3) {
        std::cerr << "Usage: " << av[0] << " <port> <password>" << std::endl;
        return 1;
    }
    try {
        signal(SIGPIPE, SIG_IGN);
        signal(SIGINT, handleShutdown);
        signal(SIGTERM, handleShutdown);
        int port = validPort(static_cast<std::string>(av[1]));
        if (port == -1) throw (std::runtime_error(RED"Error : invalid port number  => " + static_cast<std::string>(av[1])));
        if (checkPassword(static_cast<std::string>(av[2])) == -1) throw (std::runtime_error("Error : invalid password please enter a password in range of [5 to 9] printable characters!"));
        Server server(port, av[2]);
        server.run();
    } catch (const std::exception& e) {
        std::cerr << RED << "Error: " << e.what() << END << std::endl;
        return 1;
    }

    return 0;
}
