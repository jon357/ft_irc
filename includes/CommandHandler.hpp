/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcheca <jcheca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 18:14:12 by jcheca            #+#    #+#             */
/*   Updated: 2024/06/12 11:38:19 by jcheca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDHANDLER_HPP
#define COMMANDHANDLER_HPP

#include "Utils.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Who.hpp"
#include "AdditionalCommands.hpp"
#include "RPL.hpp"
#include "Join.hpp"
#include "Welcome.hpp"
#include "ModeHandler.hpp"
#include "Join.hpp"
#include "Welcome.hpp"

#include <string>
#include <sstream>
#include <algorithm>

class Server;
class Client;
class Channel;
class WhoHandler;
class AdditionalCommands;
class ModeHandler;
class JoinHandler;
class WelcomeHandler;

class CommandHandler
{
private:
	Server *_server;
	AdditionalCommands *_additionalCommands;
	ModeHandler *_modeHandler;
	JoinHandler *_joinHandler;
	WelcomeHandler *_welcomeHandler;

public:
	CommandHandler(Server *server);
	~CommandHandler();
	bool isValidNickname(const std::string& nickname);
	bool isNicknameInUse(const std::string& nickname);
	void handleNick(Client* client, const std::vector<std::string>& tokens);

	void handleUser(Client* client, const std::vector<std::string>& tokens);
	void handlePingCommand(Client* client, const std::vector<std::string>& tokens);
	void handleCommand(Client *client, const std::string &command);
	void handleCapCommand(Client* client, const std::vector<std::string>& tokens);
	void handlePassCommand(Client* client, const std::vector<std::string>& tokens);
	void handleQuitCommand(Client* client, const std::vector<std::string>& tokens);
	void handleErrorCommand(Client* client, const std::string &message);

};

#endif