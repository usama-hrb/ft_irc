#pragma once

#define RPL_WELCOME(nickname) \
    ":YourServer 001 " + nickname + " :Welcome to the Internet Relay Network " + nickname
#define RPL_YOURHOST(nickname) \
    ":YourServer 002 " + nickname + " :Your host is YourServer, running version 1.0"
#define RPL_CREATED(nickname) \
    ":YourServer 003 " + nickname + " :This server was created " + __DATE__
#define RPL_MYINFO(nickname) \
    ":YourServer 004 " + nickname + " :YourServer 1.0 - -"

#define ERR_NEEDMOREPARAMS(nickname, command) \
    ":YourServer 461 " + nickname + " " + command + " :Not enough parameters"
#define ERR_ALREADYREGISTERED(nickname) \
    ":YourServer 462 " + nickname + " :You may not reregister"
#define ERR_PASSWDMISMATCH(nickname) \
    ":YourServer 464 " + nickname + " :Password incorrect"
#define ERR_NONICKNAMEGIVEN(nickname) \
    ":YourServer 431 " + nickname + " :No nickname given"
#define ERR_ERRONEUSNICKNAME(nickname, invalid_nick) \
    ":YourServer 432 " + nickname + " " + invalid_nick + " :Erroneous nickname"
#define ERR_NICKNAMEINUSE(nickname, taken_nick) \
    ":YourServer 433 " + nickname + " " + taken_nick + " :Nickname is already in use"
#define ERR_NOTREGISTERED(nickname) \
    ":YourServer 451 " + nickname + " :You have not registered"
#define ERR_NEEDMOREPARAMS(nickname, command) \
    ":YourServer 461 " + nickname + " " + command + " :Not enough parameters"

// Channel related replies
#define ERR_NOSUCHCHANNEL(nickname, channel) \
    ":YourServer 403 " + nickname + " " + channel + " :No such channel"
#define ERR_BADCHANNELKEY(nickname, channel) \
    ":YourServer 475 " + nickname + " " + channel + " :Cannot join channel (+k)"
#define ERR_CHANOPRIVSNEEDED(nickname, channel) \
    ":YourServer 482 " + nickname + " " + channel + " :You're not channel operator"

// Mode related
#define RPL_CHANNELMODEIS(nickname, channel, modes) \
    ":YourServer 324 " + nickname + " " + channel + " " + modes
