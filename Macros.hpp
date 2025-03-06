/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Macros.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymakhlou <ymakhlou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 10:52:04 by youmoukh          #+#    #+#             */
/*   Updated: 2025/03/04 14:15:48 by ymakhlou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once


#include <fcntl.h>
# include <csignal>
# include <sys/socket.h>
# include <sys/poll.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>


# include <fstream>
# include <iostream>
# include <map>
# include <vector>
# include <cstring>
# include <cstdlib>
# include <string>
# include <sstream>
# include <algorithm>


static int socket_connection ;


# include "../Headers/Client.hpp"
# include "../Headers/Server.hpp"
# include "../Headers/Channel.hpp"

#define MAX_BUFF 100000

# define GREEN "\e[1;32m"
# define RESET "\e[0m"
# define RED "\e[1;91m"
# define CYAN "\e[1;36m"
# define YELLOW "\e[1;33m"
# define PURPLE "\e[1;35m"
# define BLUE "\e[1;34m"


# define REALNAME 99
# define USERNAME 88
# define SERVERNAME 77
# define HOSTNAME 66





#define PART_RPL(nick, channelName) (":")+ std::string(nick)+ std::string(" !~ ") + std::string(nick) + std::string(" @localhost ") + std::string(" PART ") + std::string(channelName) + std::string("\r\n");

#define ERR_UNKNOWNMODE(nick, modechar, servername) (std::string(":") + std::string(servername) + " 472 " + std::string(nick) + " " + std::string(modechar) + " :is an unknown mode char to me\r\n")

#define RPL_WELCOME(nick, hostname)  std::string("\r\n:") + std::string(hostname) + std::string(" 001 ") + std::string(nick) + std::string(" :Welcome to the Internet Relay Chat Network ") + std::string(nick) + "!~" + std::string(nick) + "@" + std::string(hostname) + "\r\n"
#define RPL_YOURHOST(nick, hostname) std::string(":") + std::string(hostname) + std::string(" 002 ") + std::string(nick) + std::string(" :Your host is ") + std::string(hostname) + std::string(", running version 1.0\r\n")
#define RPL_CREATED(nick, hostname)  std::string(":") + std::string(hostname) + std::string(" 003 ") + std::string(nick) + std::string(" :This server was created Wed Jan 01 2025\r\n")
#define RPL_MYINFO(nick, hostname)   std::string(":") + std::string(hostname) + std::string(" 004 ") + std::string(nick) + " " + std::string(hostname) + std::string(", Version: 1.0, User mode: none, Channel modes: i, t, k, o, l\r\n")


#define ERR_UNKNOWNCOMMAND(nick, hostname, command)                       std::string(":") + std::string(hostname) + " 421 " + std::string(nick) + " " + std::string(command) + " :Unknown command\r\n"
#define ERR_NEEDMOREPARAMS(nick, hostname, command)                       std::string(":") + std::string(hostname) + " 461 " + std::string(nick) + " " + std::string(command) + " :Not enough parameters\r\n"
#define ERR_PASSWDMISMATCH(nick, hostname)                                std::string(":") + std::string(hostname) + " 464 " + std::string(nick) + " :Password incorrect\r\n"
#define ERR_ALREADYREGISTERED(nick, hostname)                             std::string(":") + std::string(hostname) + " 462 " + std::string(nick) + " :You may not reregister\r\n"

#define ERR_ERRONEUSNICKNAME(nick, hostname)                              std::string(":") + std::string(hostname) + " 432 " + std::string(nick) + " :Erroneus nickname\r\n"
#define ERR_NONICKNAMEGIVEN(nick, hostname)                               std::string(":") + std::string(hostname) + " 431 " + std::string(nick) + " :No nickname given\r\n"
#define ERR_NICKNAMEINUSE(nick, hostname)                                 std::string(":") + std::string(hostname) + " 433 " + std::string(nick) + " :Nickname is already in use !\r\n"
#define RPL_NICKCHANGE(oldNick, nick, hostname)                           std::string(":") + std::string(oldNick) + " NICK "  + std::string(nick) + "\r\n"


#define ERR_BADCHANNELMASK(nick, hostname, mask, correction)              std::string(":") + std::string(hostname) + " 476 " + std::string(nick) + " " + std::string(mask) + " :Invalid channel mask." + std::string(correction) + "\r\n"
#define ERR_CHANNELISFULL(nick, hostname, channelName)                    std::string(":") + std::string(hostname) + " 471 " + std::string(nick) + " " + std::string(channelName) + " :Cannot join channel (+l) - channel is full\r\n"
#define ERR_BADCHANNELKEY(nick, hostname, channelName)                    std::string(":") + std::string(hostname) + " 475 " + std::string(nick) + " " + std::string(channelName) + " :Cannot join channel (+k) - bad key\r\n"
#define ERR_INVITEONLYCHAN(nick, hostname, channel)                       std::string(":") + std::string(hostname) + " 473 " + std::string(nick) + " " + std::string(channel) + " :Cannot join channel (+i)\r\n"


#define RPL_UMODEIS(hostname, channelname)                                std::string(":") + std::string(hostname) + " MODE " + std::string(channelname) + " +nt\r\n"
#define RPL_JOIN(nick, username, channelname, ipaddress)                  std::string(":") + std::string(nick) + "!~" + std::string(username) + "@" + std::string(ipaddress) + " JOIN " + std::string(channelname) + "\r\n"
#define RPL_TOPIC(hostname, topic, nick, channelName)                     std::string(":") + std::string(hostname) + " 332 " + std::string(nick) + " " + std::string(channelName) + " :" + std::string(topic) + "\r\n"
#define RPL_TOPICWHOTIME(topicsetter, time, nick, hostname, channelName)  std::string(":") + std::string(hostname) + " 333 " + std::string(nick) + " " + std::string(channelName) + " " + std::string(topicsetter) + "!~" + std::string(topicsetter) + "@" + std::string(hostname) + " " + std::string(time) + "\r\n"


#define RPL_NAMREPLY(hostname, clients, channelname, nick)                std::string(":") + std::string(hostname) + " 353 " + std::string(nick) + " @ " + std::string(channelname) + " :" + std::string(clients) + "\r\n"
#define RPL_ENDOFNAMES(hostname, nick, channelname)                       std::string(":") + std::string(hostname) + " 366 " + std::string(nick) + " " + std::string(channelname) + " :END of /NAMES list\r\n"


#define RPL_MODEIS(channel, hostname, mode)                               std::string(":") + std::string(hostname) + " MODE " + std::string(channel) + " " + std::string(mode) + "\r\n"
#define ERR_INVALIDMODEPARAM(channel, hostname, flag)                     std::string(":") + std::string(hostname) + " 696 " + std::string(channel) + " " + std::string(flag) + " * you must specifiy a parameter for the op mode\r\n"

#define RPL_INVITING(hostname, inviting, invited, channel)                std::string(":") + std::string(hostname) + " 341 " + std::string(inviting) + " " + std::string(invited) + " " + std::string(channel) + "\r\n"
#define RPL_INVITED(nick, hostname, invited, channel)                     std::string(":") + std::string(nick) + "!~" + std::string(hostname) + " INVITE " + std::string(invited) + " :" + std::string(channel) + "\r\n"


#define ERR_NOTONCHANNEL(hostname, nick, channel)                         std::string(":") + std::string(hostname) + " 442 " + std::string(nick) + " " + std::string(channel) + " :You're not on that channel\r\n"
#define ERR_NOSUCHCHANNEL(hostname, channel, nick)                        std::string(":") + std::string(hostname) + " 403 " + std::string(nick) + " " + std::string(channel) + " :No such channel\r\n"
#define ERR_NOSUCHNICK(hostname, channel, targetNick)                     std::string(":") + std::string(hostname) + " 401 " + std::string(channel) + " " + std::string(targetNick) + " :No such nick/channel\r\n"
#define ERR_USERONCHANNEL(hostname, channel, nick)                        std::string(":") + std::string(hostname) + " 443 " + std::string(nick) + " " + std::string(channel) + " :is already on channel\r\n"


#define ERR_INPUTTOOLONG(nick, hostname)                                  std::string(":") + std::string(hostname) + " 417 " + std::string(nick) + " :Input line was too long !\r\n"
#define RPL_BOT(hostname, nick, message)                                  std::string(":") + std::string(hostname) + " 001 " + std::string(nick) + " Dad joke: " + std::string(message) + "\r\n"


#define RPL_NOTOPIC(nick, hostname, channel)                              std::string(":") + std::string(hostname) + " 331 " + std::string(nick) + " " + std::string(channel) + " :No topic is set\r\n"

#define ERR_NOTREGISTERED(hostname, user)                                 std::string(":") + std::string(hostname) + " 451 " + std::string(user) + " :You have not registered\r\n"

// PRIVMSG

#define ERR_NORECIPIENT(nick, hostname)                                   std::string(":") + std::string(hostname) + " 411 " + std::string(nick) + " :No recipient given (PRIVMSG)\r\n"

#define ERR_NOTEXTTOSEND(nick, hostname)                                  std::string(":") + std::string(hostname) + " 412 " + std::string(nick) + " :No text to send\r\n"

#define ERR_CANNOTSENDTOCHAN(nick, hostname, channel)                     std::string(":") + std::string(hostname) + " 404 " + std::string(nick) + " " + std::string(channel) + " :Cannot send to channel\r\n"

#define ERR_NOTOPLEVEL(nick, hostname, mask)                              std::string(":") + std::string(hostname) + " 413 " + std::string(nick) + " " + std::string(mask) + " :No toplevel domain specified\r\n"

#define ERR_WILDTOPLEVEL(nick, hostname, mask)                            std::string(":") + std::string(hostname) + " 414 " + std::string(nick) + " " + std::string(mask) + " :Wildcard in toplevel domain\r\n"

#define ERR_TOOMANYTARGETS(nick, hostname, target)                        std::string(":") + std::string(hostname) + " 407 " + std::string(nick) + " " + std::string(target) + " :Too many recipients\r\n"

#define ERR_NOTAUTHENTICATED(nick, hostname)                              std::string(":") + std::string(hostname) + " 451 " + std::string(nick) + " :You have not registered\r\n"

//MODE

#define RPL_CHANNELMODEIS(nick, hostname, channel, mode)                  std::string(":") + std::string(hostname) + " 324 " + std::string(nick) + " " + std::string(channel) + " " + std::string(mode) + "\r\n"

#define ERR_USERNOTINCHANNEL(hostname, nick, nick2, channel)              std::string(":") + std::string(hostname) + " 441 " + std::string(nick) + " " + std::string(nick2) + " " + std::string(channel) + " :They aren't on that channel\r\n"

#define ERR_CHANOPRIVSNEEDED(hostname, nick, channel)                     std::string(":") + std::string(hostname) + " 482 " + std::string(nick) + " " + std::string(channel) + " :You're not channel operator\r\n"

#define ERR_CHANOPRIVSNEEDED2(hostname, nick, channel)                     std::string(":") + std::string(hostname) + " 482 " + std::string(nick) + " " + std::string(channel) + " :You cannot kick channel operator\r\n"
