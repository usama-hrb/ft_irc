

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

#define RED "\e[1;31m"
#define WHI "\e[0;37m"
#define GRE "\e[1;32m"
#define YEL "\e[1;33m"
#define END "\e[0;0m"
#define BLU "\e[1;34m"    // Bold Blue
#define MAG "\e[1;35m"    // Bold Magenta
#define CYA "\e[1;36m"    // Bold Cyan
#define BLA "\e[0;30m"    // Black (non-bold)
#define GRA "\e[1;90m"    // Bold Gray
#define PUR "\e[0;95m"    // Purple (non-bold)
#define ORA "\e[1;91m"    // Bold Orange (often a bright red shade)

#define PREFIX ":FT_irc "
#define POSTFIX "\r\n"

// Existing Codes
#define RPL_WELCOME(nickname) ":FT_irc 001 " + nickname + " :Welcome to the Internet Relay Network " + nickname + POSTFIX
#define RPL_YOURHOST(nickname) ":FT_irc 002 " + nickname + " :Your host is FT_irc, running version 1.0" + POSTFIX
#define RPL_CREATED(nickname) ":FT_irc 003 " + nickname + " :This server was created " + __DATE__ + POSTFIX
#define RPL_MYINFO(nickname) ":FT_irc 004 " + nickname + " :FT_irc 1.0 - -" + POSTFIX
#define ERR_NEEDMOREPARAMS(command) ":FT_irc 461 " + command + " :Not enough parameters" + POSTFIX
#define ERR_ALREADYREGISTRED(nickname) ":FT_irc 462 " + nickname + " :Unauthorized command (already registered)" + POSTFIX
#define ERR_PASSWDMISMATCH(nickname) ":FT_irc 464 " + nickname + " :Password incorrect" + POSTFIX
#define ERR_NONICKNAMEGIVEN(nickname) ":FT_irc 431 " + nickname + " :No nickname given" + POSTFIX
#define ERR_ERRONEUSNICKNAME(nickname, nick) ":FT_irc 432 " + nickname + " " + nick + " :Erroneus nickname" + POSTFIX
#define ERR_NICKNAMEINUSE(nickname, nick) ":FT_irc 433 " + nickname + " " + nick + " :Nickname is already in use" + POSTFIX
#define ERR_UNKNOWNCOMMAND(command) ":FT_irc 421 " + command + " :Unknown command" + POSTFIX

// New Codes for Channels
#define RPL_TOPIC(topic, nick, channelName) ":FT_irc 332 " + std::string(nick) + " " + std::string(channelName) + " :" + std::string(topic) + "\r\n"
#define RPL_NOTOPIC(nickname, channel) ":FT_irc 331 " + nickname + " " + channel + " :No topic is set" + POSTFIX
#define RPL_NAMREPLY(nickname, channel, names) ":FT_irc 353 " + nickname + " = " + channel + " :" + names + POSTFIX
#define RPL_ENDOFNAMES(nickname, channel) ":FT_irc 366 " + nickname + " " + channel + " :End of /NAMES list" + POSTFIX
#define ERR_NOTREGISTERED(none) ":FT_irc 451 " + none + " :You have not registered" + POSTFIX
#define ERR_NOSUCHCHANNEL(nickname, channel) ":FT_irc 403 " + nickname + " " + channel + " :No such channel" + POSTFIX
#define ERR_NOTONCHANNEL(channel) ":FT_irc 442 " + channel + " :You're not on that channel" + POSTFIX
#define ERR_NOSUCHNICK(nickname) ":FT_irc 401 " + nickname + " :No such nick/channel" + POSTFIX
#define RPL_LISTEND(none) ":FT_irc 323 " + none + " :END of list" + POSTFIX
#define ERR_CHANOPRIVSNEEDED(channel) ":FT_irc 482 " + channel + " :You're not channel operator" + POSTFIX
#define RPL_JOIN(nick, username, channelname, ipaddress)                  std::string(":") + std::string(nick) + "!~" + std::string(username) + "@" + std::string(ipaddress) + " JOIN " + std::string(channelname) + "\r\n"


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

	// SETTERS
	void setNickName(const std::string& nickName);
    void setUserName(const std::string& userName);
    void setRealName(const std::string& realName);
    void setPassword(bool status);
    void setIsRegistered(bool status);
};

class Channel {
private:
    std::string name;
    std::string password; //changed
    std::vector<Client*> members;
    std::vector<Client*> operators;
    std::string _topic;

public:
    void setTopic(const std::string &newTopic);
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
	bool isEmpty();
    bool removeMember(std::string nickname, std::string msg);
    void broadcast(const std::string &msg, std::string senderNick);
    std::vector<std::string> getMemberNames();
	void		print_members(){
		std::cout << "---------------------------------------------\n";
			  std::cout << "Channel Members:\n";
    for (size_t i = 0; i < members.size(); ++i) {
        if (members[i]) // Check for null pointers
            std::cout << members[i]->getNickName() << "\n";
        else
            std::cout << "[NULL MEMBER]\n";
    }
	std::cout << "---------------------------------------------\n";
		};
	void removeClient(Client* client);
};

void Channel::removeClient(Client* client) {
    members.erase(std::remove(members.begin(), members.end(), client), members.end());
    operators.erase(std::remove(operators.begin(), operators.end(), client), operators.end());
}

class ChannelManager {
public:
    std::vector<Channel*> Channels;
    Channel*	search_for_channel(std::string channel_name);
	// Client*		search_for_user(std::string nickname);
    Channel*	CreatChannel(std::string channel_name);
	void		removeChannel(const std::string& channelName);
    // Client *    getClientByNick(std::string nickname);
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

	// Command hemdlers
		void handlePass(Client* client, const std::vector<std::string>& params);
		void handleNick(Client* client, const std::vector<std::string>& params);
	    void handleUser(Client* client, const std::vector<std::string>& params);
		void handleJoin(Client* client, const std::vector<std::string>& params);
		void handleList(Client* client, const std::vector<std::string>& params);
	    void handlePrivmsg(Client* client, const std::vector<std::string>& params);
	    void handleTopic(Client* client, const std::vector<std::string>& params);
	    void handleKick(Client* client, const std::vector<std::string>& params);
	    // void handleMode(Client* client, const std::vector<std::string>& params);
	    // void handleInvite(Client* client, const std::vector<std::string>& params);
	    // void handleQuit(Client* client, const std::vector<std::string>& params);

	// Prevent copying
		Server(const Server&);
		Server& operator=(const Server&);
};



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
	std::vector<std::string> channels = split(params[0], ',') ;
	for(int i = 0; i < channels.size(); i++)
	{
		std::string channelName = channels[i];
		if (channelName[0] != '#') {
			sendReplay(client->getFd(), ERR_NOSUCHCHANNEL(client->getNickName(), channelName));
			continue;
		}
		Channel* channel = _channelManager.search_for_channel(channelName);
		int hasChannel = 0;
		if (!channel)
		{
			hasChannel = 1;
			if (params.size() > i + 1 && !params[i + 1].empty())
			{
				channel->setPassword(params[i + 1]);
			}
			channel = _channelManager.CreatChannel(channelName);
			channel->addOperator(client);
		}
		std::string password = "";
		channel->addMember(client);

		// std::string joinMsg = ":" + client->getNickName() + " JOIN :" + channelName + "\r\n";
		sendReplay(client->getFd(), RPL_JOIN(client->getNickName(), client->getUserName(), channelName, client->getClientIp()));
		channel->broadcast(RPL_JOIN(client->getNickName(), client->getUserName(), channelName, client->getClientIp()), client->getNickName());

		std::vector<std::string> names = channel->getMemberNames();
        std::string namesStr;
        for (size_t j = 0; j < names.size(); j++) {
            if (!namesStr.empty()) namesStr += " ";
            namesStr += names[j];
        }
		sendReplay(client->getFd(), RPL_TOPIC(channel->getTopic(),client->getNickName(), channelName));
        sendReplay(client->getFd(), RPL_NAMREPLY(client->getNickName(), channelName, namesStr));
        sendReplay(client->getFd(), RPL_ENDOFNAMES(client->getNickName(), channelName));
	}
}

void Server::handleList(Client* client, const std::vector<std::string>& params) {
	for (int i = 0; i < _channelManager.Channels.size(); i++)
	{
		sendReplay(client->getFd(), _channelManager.Channels[i]->getName() + "\n");
	}
	std::string response = RPL_LISTEND(std::string(""));
	sendReplay(client->getFd(), response);
}

void Server::handleKick(Client* client, const std::vector<std::string>& params)
{
    // Check for minimum required parameters (channel and nickname)
    if (params.size() < 2)
    {
        sendReplay(client->getFd(), ERR_NEEDMOREPARAMS(std::string("KICK")));
        return;
    }
	std::string reason;
    std::string channelName = params[0];
    std::string nickname = params[1];
	std::vector<std::string> messages(params.begin() + 2, params.end());
	if (params.size() > 2) {
		for (std::vector<std::string>::iterator it = messages.begin(); it != messages.end(); ++it) {
            reason += *it + " ";
        }
	} else {
		reason = "Kicked by operator";
	}
    //  = (params.size() > 2) ? params[2] : ;

    // Find the channel
    Channel* channel = _channelManager.search_for_channel(channelName);
    if (!channel)
    {
        sendReplay(client->getFd(), ERR_NOSUCHCHANNEL(client->getNickName(), channelName));
        return;
    }

    // Verify the client is an operator
    if (!channel->isOperator(client))
    {
        sendReplay(client->getFd(), ERR_CHANOPRIVSNEEDED(channelName));
        return;
    }

    // Construct the kick message in proper IRC format
    std::string kickMsg = ":" + client->getNickName() + " KICK " + channelName + " " + nickname + " " + reason + "\r\n";

    // Remove the member from the channel
    channel->removeMember(nickname, kickMsg);

    // Broadcast the kick message to all channel members
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

    // Remove the channel if it’s empty
    if (channel->isEmpty())
    {
        _channelManager.removeChannel(channelName);
    }
}

void Server::handleNick(Client* client, const std::vector<std::string>& params) {
    if (params.empty()) {
        sendReplay(client->getFd(), ERR_NONICKNAMEGIVEN(client->getNickName()));
        return;
    }
    std::string nickname = params[0];
    if (!isValidNickname(nickname)) {
        sendReplay(client->getFd(), ERR_ERRONEUSNICKNAME(client->getNickName(), nickname));
        return;
    }
    // Check nickname collision
    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->second->getNickName() == nickname) {
            sendReplay(client->getFd(), ERR_NICKNAMEINUSE(client->getNickName(), nickname));
            return;
        }
    }
    client->setNickName(nickname);
	authenticateClient(client);
}

// Utility function
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
	authenticateClient(client);
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
    // std::string message = params[1];
    std::vector<std::string> messages(params.begin() + 1, params.end());
    // std::cout << RED << messages[1] << END << std::endl;
    if(!messages.empty()) {
        std::string response = ERR_NEEDMOREPARAMS(std::string("PRIVMSG"));
        send(client->getFd(), response.c_str(), response.size() , 0);
        return;
    }

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
        std::string msgToSend = ":" + client->getNickName() + " PRIVMSG " + target + " :";
        for (std::vector<std::string>::iterator it = messages.begin(); it != messages.end(); ++it) {
            msgToSend += *it + " ";
        }
        msgToSend += "\r\n";
        std::cout << msgToSend << std::endl;
        channel->broadcast(msgToSend, client->getNickName());
    } else {
        Client* recipient = findClientByNickname(target);
        if (!recipient) {
            sendReplay(client->getFd(), ERR_NOSUCHNICK(client->getNickName()));
            return;
        }
        std::string msgToSend = ":" + client->getNickName() + " PRIVMSG " + target + " :";
        for (std::vector<std::string>::iterator it = messages.begin(); it != messages.end(); ++it) {
            msgToSend += *it + " ";
        }
        msgToSend += "\r\n";
        send(recipient->getFd(), msgToSend.c_str(), msgToSend.size(), 0);
    }
}

void Server::handleTopic(Client* client, const std::vector<std::string>& params) {
    //Check if the client is registered
    if (!client->isRegistred()) {
        sendReplay(client->getFd(), ERR_NOTREGISTERED(std::string("*")));
        return;
    }

    //Validate parameters (at least channel name required)
    if (params.empty()) {
        sendReplay(client->getFd(), ERR_NEEDMOREPARAMS(std::string("TOPIC")));
        return;
    }

    std::string channelName = params[0];
    Channel* channel = _channelManager.search_for_channel(channelName);

    //Check if the channel exists
    if (!channel) {
        sendReplay(client->getFd(), ERR_NOSUCHCHANNEL(client->getNickName(), channelName));
        return;
    }

    //Check if the client is on the channel
    if (!channel->isMember(client)) {
        sendReplay(client->getFd(), ERR_NOTONCHANNEL(channelName));
        return;
    }

   //Handle the command based on parameters
    if (params.size() == 1) {
        // Retrieve the current topic
        std::string topic = channel->getTopic();
            sendReplay(client->getFd(), RPL_TOPIC(topic, client->getNickName(), channelName));
    } else {
        // Set a new topic
        if (!channel->isOperator(client)) {
            sendReplay(client->getFd(), ERR_CHANOPRIVSNEEDED(channelName));
            return;
        }

		std::vector<std::string> vecTopic(params.begin() + 1, params.end());
        std::string newTopic;
		for (std::vector<std::string>::iterator it = vecTopic.begin(); it != vecTopic.end(); it++)
			newTopic += *it + " ";
        channel->setTopic(newTopic);

        std::string topicMsg = ":" + client->getNickName() + " TOPIC " + channelName + " :" + newTopic + "\r\n";
        channel->broadcast(topicMsg, "");
    }
}

void Server::handleUser(Client *client, const std::vector<std::string> &params) {
	std::string response;
	if (client->isRegistred()) {
		response = ERR_ALREADYREGISTRED(std::string("*"));
		sendReplay(client->getFd(), response);
		return;
	}
	if (params.size() < 4) {
		response = ERR_NEEDMOREPARAMS(std::string("USER"));
		sendReplay(client->getFd(), response);
		return;
	}
	client->setUserName(params[0]);
	client->setRealName(params[3].substr(1));
	authenticateClient(client);
}

Channel::Channel(std::string new_name) : name(new_name), _topic("No topic is set") {}

size_t Channel::getMemrbersNum() {return members.size();}

void Channel::setTopic(const std::string &newTopic) {
    if (newTopic.size() > 100) {
        _topic = newTopic.substr(0, 100);
    } else {
       _topic = newTopic;
    }
}


std::string Channel::getTopic() const {
    return _topic;
}

std::string Channel::getName() {
    return name;
}

std::string Channel::getPassword() {return password;} //changed

void Channel::setPassword(std::string password) {password = password;} //changed

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

void Channel::broadcast(const std::string &msg, std::string senderNick){
    for (size_t i = 0; i < members.size(); ++i) {
        if (members[i]->getNickName() != senderNick) {
            send(members[i]->getFd(), msg.c_str(), msg.size(), 0);
        }
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

Client::~Client() {}

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
            std::cout << RED << "Client with fd : " << clientFd << " disconnected" << END << std::endl;
            closeClient(clientFd);
            return;
        } 
        else if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return;
        } 
        else {
            std::cerr << "recv error: " << strerror(errno) << std::endl;
            closeClient(clientFd);
            return;
        }
    }

    buffer[bytesRead] = '\0';
    std::string message(buffer);
    std::cout << "-> " << message << std::endl;

    Client* client = _clients[clientFd];
    
    std::istringstream iss(message);
    std::string commandLine;
    while(std::getline(iss, commandLine)) {
        commandLine = commandLine.substr(0, commandLine.find_last_not_of("\r\n") + 1);
        processCommand(client, commandLine);
		usleep(100);
    }
}


void Server::processCommand(Client* client, const std::string& command) {
	std::istringstream iss(command);
	std::vector<std::string> params;
	std::string cmd;
	std::string response;

	iss >> cmd;
	for (std::string param; iss >> param; )
		params.push_back(param);
	if (cmd == "PASS" || cmd == "pass") handlePass(client, params);
	else if (cmd == "NICK" || cmd == "nick") handleNick(client, params);
	else if (cmd == "USER" || cmd == "user") handleUser(client, params);
	else if (cmd == "JOIN" || cmd == "join") handleJoin(client, params);
	else if (cmd == "LIST" || cmd == "list") handleList(client, params);
	else if (cmd == "KICK" || cmd == "kick") handleKick(client, params);
	else if (cmd == "TOPIC" || cmd == "topic") handleTopic(client, params);
	else if (cmd == "PRIVMSG" || cmd == "privmsg") handlePrivmsg(client, params);
	else if (cmd != "PONG" && cmd != "PING") {
		response = ERR_UNKNOWNCOMMAND(cmd);
		sendReplay(client->getFd(), response);
	}
}

// void Server::closeClient(int clientFd) {
//     // Find the client
//     std::map<int, Client*>::iterator it = _clients.find(clientFd);
//     Client* client = (it != _clients.end()) ? it->second : NULL;

//     if (client) {
//         // Remove client from all channels
//         std::vector<std::string> channelsToRemove;
//         for (std::vector<Channel*>::iterator chanIt = _channelManager.Channels.begin(); chanIt != _channelManager.Channels.end(); ++chanIt) {
//             (*chanIt)->removeClient(client);
//             if ((*chanIt)->isEmpty()) {
//                 channelsToRemove.push_back((*chanIt)->getName());
//             }
//         }
//         // Remove empty channels after iteration to avoid iterator invalidation
//         for (std::vector<std::string>::iterator nameIt = channelsToRemove.begin(); nameIt != channelsToRemove.end(); ++nameIt) {
//             _channelManager.removeChannel(*nameIt);
//         }
//     }
//     // Remove from pollfds
//     for (std::vector<pollfd>::iterator pollIt = _pollFds.begin(); pollIt != _pollFds.end(); ++pollIt) {
//         if (pollIt->fd == clientFd) {
//             _pollFds.erase(pollIt);
//             break;
//         }
//     }
//     // Delete client and remove from map
//     if (client) {
//         delete client;
//         _clients.erase(clientFd);
//     }
//     close(clientFd);
// }


void Server::closeClient(int clientFd) {
    std::map<int, Client*>::iterator it = _clients.find(clientFd);
    if (it == _clients.end()) return;

    Client* client = it->second;
    
    // Remove client from all channels
    std::vector<std::string> channelsToRemove;
    for (std::vector<Channel*>::iterator chanIt = _channelManager.Channels.begin(); chanIt != _channelManager.Channels.end(); ++chanIt) {
        if ((*chanIt)->isMember(client)) {
            (*chanIt)->removeClient(client);
            if ((*chanIt)->isEmpty()) {
                channelsToRemove.push_back((*chanIt)->getName());
            }
        }
    }

    // Delete empty channels
    for (size_t i = 0; i < channelsToRemove.size(); ++i) {
        _channelManager.removeChannel(channelsToRemove[i]);
    }

    // Remove from pollfds
    for (std::vector<pollfd>::iterator pollIt = _pollFds.begin(); pollIt != _pollFds.end(); ++pollIt) {
        if (pollIt->fd == clientFd) {
            _pollFds.erase(pollIt);
            break;
        }
    }

    // Remove from client map
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

Server::Server(const Server &obj) {

}

Server &Server::operator= (const Server &obj) {

	return (*this);
}

void Server::authenticateClient(Client *client) {
	if (client->hasPassword() && client->hasNickName() && client->hasUserName() && !client->isRegistred()) {
		client->setIsRegistered(true);
		welcomingMessage(client);
	}
}

void Server::welcomingMessage(Client* client) const {
	sendReplay(client->getFd(), RPL_WELCOME(client->getNickName()));
	sendReplay(client->getFd(), RPL_YOURHOST(client->getNickName()));
	sendReplay(client->getFd(), RPL_CREATED(client->getNickName()));
	sendReplay(client->getFd(), RPL_MYINFO(client->getNickName()));
}

void Server::sendReplay(int fd, const std::string& response) const {
	std::string formatted = response;
    send(fd, formatted.c_str(), formatted.size(), 0);
}

void Server::run() {
	_running = true;
	_pollFds.push_back((pollfd){_serverFd, POLLIN, 0});

	while (_running) {
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

void f() {
	system("leaks ircserv");
	system("lsof -c ircserv");
}

int main(int ac, char** av) {
    if (ac != 3) {
        std::cerr << "Usage: " << av[0] << " <port> <password>" << std::endl;
        return 1;
    }
    try {
		signal(SIGPIPE, SIG_IGN);
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