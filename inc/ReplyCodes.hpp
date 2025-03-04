#pragma once

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
#define RPL_NOTOPIC(nickname, channel) ":FT_irc 331 " + nickname + " " + channel + " :No topic is set" + POSTFIX
#define RPL_NAMREPLY(nickname, channel, names) ":FT_irc 353 " + nickname + " = " + channel + " :" + names + POSTFIX
#define RPL_ENDOFNAMES(nickname, channel) ":FT_irc 366 " + nickname + " " + channel + " :End of /NAMES list" + POSTFIX
#define ERR_NOTREGISTERED(none) ":FT_irc 451 " + none + " :You have not registered" + POSTFIX
#define ERR_NOSUCHCHANNEL(nickname, channel) ":FT_irc 403 " + nickname + " " + channel + " :No such channel" + POSTFIX