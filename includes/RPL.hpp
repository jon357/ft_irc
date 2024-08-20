/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPL.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcheca <jcheca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 15:12:47 by jcheca            #+#    #+#             */
/*   Updated: 2024/06/08 20:20:39 by jcheca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RPL_HPP
#define RPL_HPP

#include <string>
#include <sstream>

#include "Client.hpp"
#include "Utils.hpp"
#include "Channel.hpp"

#define SERVER_NAME "IRC_Server"
#define SERVER_VERSION "1.0"
#define USER_MODES "None"
#define CHANNEL_MODES "l"
#define CHANNEL_MODES_WITH_PARAMS "l"


#define CLIENT_FD(client) (client->getFd())
#define CLIENT_NICK(client) ((client)->getNickname())
#define CLIENT_USER(client) ((client)->getUser())
#define CLIENT_HOST(client) ((client)->getHost())
#define CLIENT_REALNAME(client) ((client)->getRealName())

inline std::string RPL_WELCOME(Client* client)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 001 " << CLIENT_NICK(client)
		<< " :Welcome to the Internet Relay Network " << CLIENT_NICK(client)
		<< "!" << CLIENT_USER(client) << "@" << CLIENT_HOST(client) << "\r\n";
	return oss.str();
}

inline std::string RPL_YOURHOST(Client* client)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 002 " << CLIENT_NICK(client)
		<< " :Your host is " << SERVER_NAME << ", running version " << SERVER_VERSION << "\r\n";
	return oss.str();
}

inline std::string RPL_CREATED(Client* client)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 003 " << CLIENT_NICK(client)
		<< " :This server was created " << __DATE__ << "\r\n";
	return oss.str();
}

inline std::string RPL_MYINFO(Client* client)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 004 " << CLIENT_NICK(client) << " "
		<< SERVER_NAME << " " << SERVER_VERSION << " "
		<< USER_MODES << " " << CHANNEL_MODES << " "
		<< CHANNEL_MODES_WITH_PARAMS << "\r\n";
	return oss.str();
}

inline std::string RPL_ISUPPORT(Client* client, const std::string& tokens)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 005 " << CLIENT_NICK(client)
		<< " " << tokens << " :are supported by this server\r\n";
	return oss.str();
}

inline std::string RPL_UMODEIS(Client* client, const std::string& modes)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 221 " << CLIENT_NICK(client) << " :" << modes << "\r\n";
	return oss.str();
}

inline std::string RPL_AWAY(Client *client, const std::string& target, const std::string& message)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 301 " << CLIENT_NICK(client) << " " << target << " :" << message << "\r\n";
	return oss.str();
}

inline std::string RPL_WHOISUSER(Client *client, Client* target)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 311 " << CLIENT_NICK(client) << " " << CLIENT_NICK(target) << " "
		<< CLIENT_USER(target) << " " << CLIENT_HOST(target) << " * :" << CLIENT_REALNAME(target) << "\r\n";
	return oss.str();
}

inline std::string RPL_WHOISSERVER(Client *client, const std::string& targetNick, const std::string& serverInfo)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 312 " << CLIENT_NICK(client) << " " << targetNick << " " << SERVER_NAME
		<< " :" << serverInfo << "\r\n";
	return oss.str();
}

inline std::string RPL_WHOISOPERATOR(Client* client, const std::string& nick)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 313 " << CLIENT_NICK(client) << " " << nick << " :is an IRC operator\r\n";
	return oss.str();
}

inline std::string RPL_ENDOFWHO(Client *client, const std::string& channel)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 315 " << CLIENT_NICK(client) << " " << channel << " :End of /WHO list.\r\n";
	return oss.str();
}

inline std::string RPL_ENDOFWHOIS(Client *client, const std::string& targetNick)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 318 " << CLIENT_NICK(client) << " " << targetNick << " :End of /WHOIS list.\r\n";
	return oss.str();
}

inline std::string RPL_LIST(Client *client, const std::string& channel, int numVisible, const std::string& topic)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 322 " << CLIENT_NICK(client) << " " << channel << " " << numVisible << " :" << topic << "\r\n";
	return oss.str();
}

inline std::string RPL_LISTEND(Client *client)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 323 " << CLIENT_NICK(client) << " :End of /LIST\r\n";
	return oss.str();
}

inline std::string RPL_CHANNELMODEIS(int clientFd, const std::string& channel, const std::string& mode)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 324 " << clientFd << " " << channel << " " << mode << "\r\n";
	return oss.str();
}

inline std::string RPL_CREATIONTIME(Client *client, const std::string& channel, time_t creationTime)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 329 " << CLIENT_NICK(client) << " " << channel << " " << creationTime << "\r\n";
	return oss.str();
}

inline std::string RPL_NOTOPIC(Client* client, const std::string& channel)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 331 " << CLIENT_NICK(client) << " " << channel << " :No topic is set\r\n";
	return oss.str();
}

inline std::string RPL_TOPIC(Client* client, const std::string& channel, const std::string& topic)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 332 " << CLIENT_NICK(client) << " " << channel << " :" << topic << "\r\n";
	return oss.str();
}

inline std::string RPL_TOPICWHOTIME(Client* client, const std::string& channel, const std::string& setter, time_t setTime)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 333 " << CLIENT_NICK(client) << " " << channel << " " << setter << " " << setTime << "\r\n";
	return oss.str();
}

inline std::string RPL_INVITELIST(Client *client, const std::string& channel, const std::string& inviteMask)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 336 " << CLIENT_NICK(client) << " " << channel << " " << inviteMask << "\r\n";
	return oss.str();
}

inline std::string RPL_ENDOFINVITELIST(Client *client, const std::string& channel)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 337 " << CLIENT_NICK(client) << " " << channel << " :End of channel invite list\r\n";
	return oss.str();
}

inline std::string RPL_INVITING(Client* client, Client* clientTarget, const std::string& channel)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 341 " << CLIENT_NICK(client) << " " << CLIENT_NICK(clientTarget) << " " << channel << "\r\n";
	return oss.str();
}

inline std::string RPL_EXCEPTLIST(Client *client, const std::string& channel, const std::string& exceptionMask)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 348 " << CLIENT_NICK(client) << " " << channel << " " << exceptionMask << "\r\n";
	return oss.str();
}

inline std::string RPL_ENDOFEXCEPTLIST(Client *client, const std::string& channel)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 349 " << CLIENT_NICK(client) << " " << channel << " :End of channel exception list\r\n";
	return oss.str();
}

inline std::string RPL_NAMREPLY(Client* client, const std::string& channel, const std::string& users)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 353 " << CLIENT_NICK(client) << " = " << channel << " :" << users << "\r\n";
	return oss.str();
}

inline std::string RPL_WHOREPLY(const std::string& channel, Client* target)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 352 " << CLIENT_NICK(target) << " " << channel << " "
		<< CLIENT_USER(target) << " " << CLIENT_HOST(target) << SERVER_NAME << " "
		<< CLIENT_NICK(target) << " H :0 " << CLIENT_REALNAME(target) << "\r\n";
	return oss.str();
}

inline std::string RPL_ENDOFNAMES(Client* client, const std::string& channel)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 366 " << CLIENT_NICK(client) << " " << channel << " :End of /NAMES list\r\n";
	return oss.str();
}

inline std::string RPL_BANLIST(Client *client, const std::string& channel, const std::string& banMask)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 367 " << CLIENT_NICK(client) << " " << channel << " " << banMask << "\r\n";
	return oss.str();
}

inline std::string RPL_ENDOFBANLIST(Client *client, const std::string& channel)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 368 " << CLIENT_NICK(client) << " " << channel << " :End of channel ban list\r\n";
	return oss.str();
}

inline std::string RPL_MOTD(Client* client, const std::string& line)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 372 " << CLIENT_NICK(client)
		<< " :- " << line << "\r\n";
	return oss.str();
}

inline std::string RPL_MOTDSTART(Client* client)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 375 " << CLIENT_NICK(client)
		<< " :- " << SERVER_NAME << " Message of the day - \r\n";
	return oss.str();
}

inline std::string RPL_ENDOFMOTD(Client* client)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 376 " << CLIENT_NICK(client)
		<< " :End of /MOTD command.\r\n";
	return oss.str();
}

inline std::string ERR_NOSUCHNICK(Client *client, const std::string& nick)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 401 " << CLIENT_NICK(client) << " " << nick << " :No such nick/channel\r\n";
	return oss.str();
}

inline std::string ERR_NOSUCHCHANNEL(Client *client, const std::string& channel)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 403 " << CLIENT_NICK(client) << " " << channel << " :No such channel\r\n";
	return oss.str();
}

inline std::string ERR_CANNOTSENDTOCHAN(Client *client, const std::string& channel)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 404 " << CLIENT_NICK(client) << " " << channel << " :Cannot send to channel\r\n";
	return oss.str();
}

inline std::string ERR_TOOMANYCHANNELS(Client* client, const std::string& channel)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 405 " << CLIENT_NICK(client) << " " << channel << " :You have joined too many channels\r\n";
	return oss.str();
}

// RPL Error Messages
inline std::string ERR_NOORIGIN(Client* client)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 409 " << CLIENT_NICK(client) << " :No origin specified\r\n";
	return oss.str();
}

inline std::string ERR_NORECIPIENT(Client *client, const std::string& command)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 411 " << CLIENT_NICK(client) << " :No recipient given (" << command << ")\r\n";
	return oss.str();
}

inline std::string ERR_NOTEXTTOSEND(Client *client)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 412 " << CLIENT_NICK(client) << " :No text to send\r\n";
	return oss.str();
}

inline std::string ERR_UNKNOWNCOMMAND(Client* client, const std::string& command)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 421 " << CLIENT_NICK(client) << " " << command << " :Unknown command\r\n";
	return oss.str();
}

inline std::string ERR_NOMOTD(Client* client)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 422 " << CLIENT_NICK(client)
		<< " :MOTD File is missing\r\n";
	return oss.str();
}

inline std::string ERR_NONICKNAMEGIVEN(Client* client)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 431 " << CLIENT_NICK(client) << " :No nickname given\r\n";
	return oss.str();
}

inline std::string ERR_ERRONEUSNICKNAME(Client* client, const std::string& nickname)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 432 " << CLIENT_NICK(client) << " " << nickname << " :Erroneous nickname\r\n";
	return oss.str();
}

inline std::string ERR_NICKNAMEINUSE(Client* client, const std::string& nickname)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 433 " << CLIENT_NICK(client) << " " << nickname << " :Nickname is already in use\r\n";
	return oss.str();
}

inline std::string ERR_USERNOTINCHANNEL(Client *client, const std::string& nick, const std::string& channel)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 441 " << CLIENT_NICK(client) << " " << nick << " " << channel
		<< " :They aren't on that channel\r\n";
	return oss.str();
}

inline std::string ERR_NOTONCHANNEL(Client *client, const std::string& channel)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 442 " << CLIENT_NICK(client) << " " << channel << " :You're not on that channel\r\n";
	return oss.str();
}

// Ajoutez cette fonction pour ERR_USERONCHANNEL
inline std::string ERR_USERONCHANNEL(Client* client, const std::string& nick, const std::string& channel)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 443 " << CLIENT_NICK(client) << " " << nick << " " << channel
		<< " :is already on channel\r\n";
	return oss.str();
}

inline std::string ERR_NOTREGISTERED(Client* client)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 451 " << CLIENT_NICK(client) << " :You have not registered\r\n";
	return oss.str();
}

//RPL Password
// RPL Error Messages
inline std::string ERR_NEEDMOREPARAMS(Client* client, std::string channel, const std::string& command)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 461 " << CLIENT_NICK(client) << " " << channel << command << " :Not enough parameters\r\n";
	return oss.str();
}

inline std::string ERR_ALREADYREGISTERED(Client* client)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 462 " << CLIENT_NICK(client) << " :You may not reregister\r\n";
	return oss.str();
}

inline std::string ERR_PASSWDMISMATCH(Client* client)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 464 " << CLIENT_NICK(client) << " :Password incorrect\r\n";
	return oss.str();
}

inline std::string ERR_INVALIDKEY(Client* client, const std::string& channel)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 467 " << CLIENT_NICK(client) << " " << channel << " :Invalid key\r\n";
	return oss.str();
}

inline std::string ERR_KEYSET(Client* client, const std::string& channel)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 467 " << CLIENT_NICK(client) << " " << channel
		<< " :Channel key already set\r\n";
	return oss.str();
}

inline std::string ERR_LINKSET(Client* client, const std::string& channel)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 469 " << CLIENT_NICK(client) << " " << channel
		<< " :No key set\r\n";
	return oss.str();
}

inline std::string ERR_CHANNELISFULL(Client* client, const std::string& channel)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 471 " << CLIENT_NICK(client) << " " << channel << " :Cannot join channel (channel is full)\r\n";
	return oss.str();
}

inline std::string ERR_UNKNOWNMODE(Client* client, char mode, const std::string& channelName)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 472 " << CLIENT_NICK(client) << " " << mode << " :is unknown mode char to me for " << channelName << "\r\n";
	return oss.str();
}

inline std::string ERR_INVITEONLYCHAN(Client* client, const std::string& channel)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 473 " << CLIENT_NICK(client) << " " << channel << " :Cannot join channel (invite only)\r\n";
	return oss.str();
}

inline std::string ERR_BANNEDFROMCHAN(Client* client, const std::string& channel)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 474 " << CLIENT_NICK(client) << " " << channel << " :Cannot join channel (banned)\r\n";
	return oss.str();
}

inline std::string ERR_BADCHANNELKEY(Client* client, const std::string& channel)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 475 " << CLIENT_NICK(client) << " " << channel << " :Cannot join channel (incorrect key)\r\n";
	return oss.str();
}

inline std::string ERR_BADCHANMASK(Client* client, const std::string& channel)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 476 " << CLIENT_NICK(client) << " " << channel << " :Bad channel mask\r\n";
	return oss.str();
}

inline std::string RPL_NOCHANMODES(Client *client, const std::string& channel)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 477 " << CLIENT_NICK(client) << " " << channel << " :Channel doesn't support modes\r\n";
	return oss.str();
}

inline std::string ERR_CHANOPRIVSNEEDED(Client *client, const std::string& channel)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 482 " << CLIENT_NICK(client) << " " << channel << " :You're not channel operator\r\n";
	return oss.str();
}

inline std::string ERR_UMODEUNKNOWNFLAG(Client *client)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 501 " << CLIENT_NICK(client) << " :Unknown MODE flag\r\n";
	return oss.str();
}

inline std::string ERR_USERSDONTMATCH(Client *client)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 502 " << CLIENT_NICK(client) << " :Cannot change mode for other users\r\n";
	return oss.str();
}

inline std::string ERR_INVALIDMODEPARAM(Client* client, const std::string& param)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " 696 " << CLIENT_NICK(client) << " " << param << " :Invalid mode parameter\r\n";
	return oss.str();
}

inline std::string RPL_PONG(const std::string& token)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " PONG " << SERVER_NAME << " " << token << "\r\n";
	return oss.str();
}

inline std::string RPL_CAP(Client *client, const std::string& subcommand, const std::string& capabilities)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " CAP " << CLIENT_NICK(client) << " " << subcommand << " :" << capabilities << "\r\n";
	return oss.str();
}

inline std::string RPL_PASSACCEPTED(Client* client)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " NOTICE " << CLIENT_NICK(client)
		<< " :Password accepted!\r\n";
	return oss.str();
}

inline std::string RPL_CAPEND(Client *client)
{
	std::ostringstream oss;
	oss << ":" << SERVER_NAME << " CAP " << CLIENT_NICK(client) << " END\r\n";
	return oss.str();
}

inline std::string MODEACCEPTMESSAGE(Client *client, std::string channel, const std::string& mode, const std::string& argument)
{
	std::ostringstream oss;
	oss << ":" << client->getNickname() << " MODE " << channel << " " << mode << argument << "\r\n";
	return oss.str();
}

inline std::string BOTMESSAGE(Client *client, const std::string &channel, const std::string& message)
{
	std::ostringstream oss;
	oss << ":" << client->getNickname() << " NOTICE " << channel << " :" << message << "\r\n";
	return oss.str();
}

inline std::string MODELNEEDPARAM(const std::string& mode)
{
	std::ostringstream oss;
	oss << mode << ":NOT enought parameters" << "\r\n";
	return oss.str();
}

#endif
