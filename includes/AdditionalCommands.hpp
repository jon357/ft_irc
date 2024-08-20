/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AdditionalCommands.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcheca <jcheca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:09:05 by jcheca            #+#    #+#             */
/*   Updated: 2024/06/12 11:39:51 by jcheca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ADDITIONALCOMMANDS_HPP
#define ADDITIONALCOMMANDS_HPP

#include "Client.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "CommandHandler.hpp"
#include "Utils.hpp"
#include "RPL.hpp"
#include "Who.hpp"
#include "InviteHandler.hpp"
#include "TopicHandler.hpp"
#include "KickHandler.hpp"
#include "BotFilter.hpp"
#include "Join.hpp"

#include <sstream>
#include <string>
#include <fstream>

class Server;
class Client;
class Channel;
class BotFilter;
class KickHandler;
class AdditionalCommands
{
	private:
		Server *_server;

	public:
		AdditionalCommands(Server *server);
		~AdditionalCommands();
		void processCommand(Client *client, const std::string &command);
		void broadcastChannelList(Client *client, Server *server);
		void handlePartCommand(Server *server, Client *client, const std::string &command);
		void handleNickCommand(Server *server, Client *client, const std::string &command);
		void handlePrivmsgCommand(Server *server, Client *client, const std::string &command);
};

#endif
