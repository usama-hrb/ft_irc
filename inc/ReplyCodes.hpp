#pragma once
#include "Needs.hpp"

#define PREFIX ":FT_irc "
#define POSTFIX "\r\n"

// Existing Codes
#define RPL_WELCOME(nickname) PREFIX " 001 " + nickname + " :Welcome to the Internet Relay Network " + nickname + POSTFIX
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
#define ERR_INVITEONLYCHAN(nick, channel)                       std::string(":") + PREFIX + " 473 " + std::string(nick) + " " + std::string(channel) + " :Cannot join channel (+i)\r\n"
#define ERR_CHANNELISFULL(nick, channelName)                    std::string(":") + PREFIX + " 471 " + std::string(nick) + " " + std::string(channelName) + " :Cannot join channel (+l) - channel is full\r\n"
#define ERR_NOCHANMODES(nickname, channel) "477 " + nickname + " " + channel + " :Channel doesn't support modes"

// New Codes for Channels
#define RPL_TOPIC(topic, nick, channelName) PREFIX " 332 " + std::string(nick) + " " + std::string(channelName) + " :" + std::string(topic) + "\r\n"
#define RPL_NOTOPIC(nickname, channel) PREFIX " 331 " + nickname + " " + channel + " :No topic is set" + POSTFIX
#define RPL_NAMREPLY(nickname, channel, names) PREFIX " 353 " + nickname + " = " + channel + " :" + names + POSTFIX
#define RPL_ENDOFNAMES(nickname, channel) PREFIX " 366 " + nickname + " " + channel + " :End of /NAMES list" + POSTFIX
#define ERR_NOTREGISTERED(none) PREFIX " 451 " + none + " :You have not registered" + POSTFIX
#define ERR_NOSUCHCHANNEL(nickname, channel) PREFIX " 403 " + nickname + " " + channel + " :No such channel" + POSTFIX
#define ERR_NOTONCHANNEL(channel) PREFIX " 442 " + channel + " :You're not on that channel" + POSTFIX
#define ERR_NOSUCHNICK(nickname) PREFIX " 401 " + nickname + " :No such nick/channel" + POSTFIX
#define RPL_LISTEND(none) PREFIX " 323 " + none + " :END of list" + POSTFIX
#define ERR_CHANOPRIVSNEEDED(channel) PREFIX " 482 " + channel + " :You're not channel operator" + POSTFIX
#define RPL_JOIN(nick, username, channelname, ipaddress)                  std::string(":") + std::string(nick) + "!~" + std::string(username) + "@" + std::string(ipaddress) + " JOIN " + std::string(channelname) + "\r\n"
#define RPL_INVITING(inviting, invited, channel)                std::string(":") + PREFIX + " 341 " + std::string(inviting) + " " + std::string(invited) + " " + std::string(channel) + "\r\n"
#define RPL_INVITED(nick, invited, channel, info)                     std::string(":") + info + " INVITE " + std::string(invited) + " :" + std::string(channel) + "\r\n"
#define RPL_QUIT(nickname, message) ":" + nickname + " QUIT : " + message + "\r\n"
#define RPL_OPERATOR(nickname) ":" + nickname + " IS OPERATOR" "\r\n"

#define SERV() "\n\
░██████╗███████╗██████╗░██╗░░░██╗███████╗██████╗░\n\
██╔════╝██╔════╝██╔══██╗██║░░░██║██╔════╝██╔══██╗\n\
╚█████╗░█████╗░░██████╔╝╚██╗░██╔╝█████╗░░██████╔╝\n\
░╚═══██╗██╔══╝░░██╔══██╗░╚████╔╝░██╔══╝░░██╔══██╗\n\
██████╔╝███████╗██║░░██║░░╚██╔╝░░███████╗██║░░██║\n\
╚═════╝░╚══════╝╚═╝░░╚═╝░░░╚═╝░░░╚══════╝╚═╝░░╚═╝\n"
