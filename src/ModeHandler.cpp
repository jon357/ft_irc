/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeHandler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcheca <jcheca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 11:13:08 by jcheca            #+#    #+#             */
/*   Updated: 2024/06/12 11:11:16 by jcheca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ModeHandler.hpp"

ModeHandler::ModeHandler(Server* server) : _server(server) {}

ModeHandler::~ModeHandler() {}

void ModeHandler::handleModeCommand(Client* client, const std::string& command)
{
	_server->log("Received MODE command: " + command, BLUE);
	std::vector<std::string> tokens = split(command, " \n\r\t");

	if (tokens.size() < 2)
	{
		_server->sendToClient(client->getFd(), ERR_NEEDMOREPARAMS(client, NULL, "MODE"));
		_server->log("MODE command error: Need more parameters", RED);
		return;
	}

	std::string target = tokens[1];
	if (target[0] == '#' || target[0] == '&')
	{
		_server->log("Handling channel mode for target: " + target, GREEN);
		handleChannelMode(client, tokens);
	}
	else
	{
		_server->log("Handling user mode for target: " + target, GREEN);
		handleUserMode(client, tokens);
	}
}

void ModeHandler::handleUserMode(Client* client, const std::vector<std::string>& tokens)
{
	_server->log("Entered handleUserMode", MAGENTA);
	std::string target = tokens[1];
	_server->log("User mode target: " + target, CYAN);
	if (target != client->getNickname())
	{
		_server->sendToClient(client->getFd(), ERR_USERSDONTMATCH(client));
		_server->log("User mode error: Users don't match", RED);
		return;
	}

	if (tokens.size() == 2)
	{
		_server->sendToClient(client->getFd(), RPL_UMODEIS(client, ""));
		_server->log("Sent user modes to client", GREEN);
	}
	else
	{
		std::string modeString = tokens[2];
		_server->log("Updating user modes: " + modeString, BLUE);
	}
}

void ModeHandler::handleChannelMode(Client* client, const std::vector<std::string>& tokens)
{
	_server->log("Entered handleChannelMode", MAGENTA);
	std::string channelName = tokens[1];
	_server->log("Channel mode target: " + channelName, CYAN);
	Channel* channel = _server->getChannelByName(channelName);
	if (!channel)
	{
		_server->sendToClient(client->getFd(), ERR_NOSUCHCHANNEL(client, channelName));
		_server->log("Channel mode error: No such channel " + channelName, RED);
		return;
	}

	if (tokens.size() == 2)
	{
		std::ostringstream oss;
		oss << RPL_CHANNELMODEIS(client->getFd(), channel->getName(), channel->getModes());
		oss << RPL_CREATIONTIME(client, channel->getName(), channel->getTopicTime());
		_server->sendToClient(client->getFd(), oss.str());
		_server->log("Sent channel modes to client", GREEN);
		return;
	}

	if (!channel->isOperator(client))
	{
		_server->sendToClient(client->getFd(), ERR_CHANOPRIVSNEEDED(client, channelName));
		_server->log("Channel mode error: Channel operator privileges needed for " + channelName, RED);
		return;
	}

	std::string modeString = tokens[2];
	bool adding = true;
	size_t argIndex = 3;

	for (size_t i = 0; i < modeString.length(); ++i)
	{
		char mode = modeString[i];
		if (mode == '+')
		{
			adding = true;
			_server->log("Adding mode: " + std::string(1, mode), GREEN);
		}
		else if (mode == '-')
		{
			adding = false;
			_server->log("Removing mode: " + std::string(1, mode), RED);
		}
		else
		{
			std::string argument;
			if (needsArgument(mode) && argIndex < tokens.size())
			{
				argument = tokens[argIndex++];
			}
			setChannelMode(client, channel, std::string(1, mode), adding, argument);
		}
	}
}

void ModeHandler::setChannelMode(Client *client, Channel* channel, const std::string& mode, bool adding, const std::string& argument)
{
	_server->log("Setting channel mode: " + mode + " Adding: " + (adding ? "true" : "false") + " Argument: " + argument, GREEN);
	if (mode == "l")
		{
			applyModeL(client, channel, adding, argument);
		}
	else if (mode == "i")
		{
			applyModeI(client, channel, adding);
		}
	else if (mode == "k")
		{
			applyModeK(client, channel, adding, argument);
		}
	else if (mode == "t")
		{
			applyModeT(client, channel, adding);
		}
	else if (mode == "o")
			applyModeO(client, channel, adding, argument);
}

void ModeHandler::applyModeL(Client *client, Channel* channel, bool adding, const std::string& argument)
{
	if (adding)
	{
		int limit = std::atoi(argument.c_str());
		if (limit <= 0)
		{
			_server->sendToClient(client->getFd(), MODELNEEDPARAM("MODE +l"));
			_server->log("Invalid limit for mode +l: " + argument, RED);
			return;
		}
		std::ostringstream oss;
		oss << limit;
		channel->broadcast(MODEACCEPTMESSAGE(client, channel->getName(), "+l", "" + argument), NULL, _server);
		_server->log("Applying mode L: Setting limit to " + oss.str(), GREEN);
		channel->setClientLimit(limit);
	}
	else
	{
		channel->broadcast(MODEACCEPTMESSAGE(client, channel->getName(), "-l", "" + argument), NULL, _server);
		_server->log("Applying mode L: Removing limit", RED);
		channel->setClientLimit(0);
	}
}

void	ModeHandler::applyModeI(Client *client, Channel *channel, bool adding)
{
	std::string	modeChange;
	bool		isAlreadySet;

	modeChange = adding ? "+i" : "-i";
	isAlreadySet = channel->isInviteOnly() == adding;
	if (!isAlreadySet)
	{
		channel->broadcast(MODEACCEPTMESSAGE(client, channel->getName(), modeChange, ""), NULL, _server);
		_server->log("Applying mode I: " + std::string(adding ? "Setting invite-only" : "Removing invite-only"), GREEN);
		channel->setInviteOnly(adding);
	}
}

void	ModeHandler::applyModeK(Client *client, Channel *channel, bool adding, const std::string &argument)
{
	bool	isAlreadyProtected;

	isAlreadyProtected = !channel->getKey().empty();
	if (adding)
	{
		if (isAlreadyProtected)
		{
			_server->sendToClient(client->getFd(), ERR_KEYSET(client, channel->getName()));
			_server->log("Mode +k error: Channel already has a key set", RED);
			return;
		}
		if (argument.empty())
		{
			_server->sendToClient(client->getFd(), MODELNEEDPARAM("MODE +k"));
			_server->log("Mode +k error: No key provided", RED);
			return;
		}
		if (argument.find(' ') != std::string::npos)
		{
			_server->sendToClient(client->getFd(), ERR_INVALIDMODEPARAM(client, argument));
			_server->log("Invalid key for mode +k: contains spaces", RED);
			return;
		}
		channel->broadcast( MODEACCEPTMESSAGE(client, channel->getName(), "+k ", argument), NULL, _server);
		_server->log("Applying mode K: Setting key to " + argument, GREEN);
		channel->setKey(argument);
	}
	else
	{
		if (!isAlreadyProtected)
		{
			_server->sendToClient(client->getFd(), ERR_LINKSET(client, channel->getName()));
			_server->log("Mode -k error: No key to remove", RED);
			return;
		}
		_server->sendToClient(client->getFd(), MODELNEEDPARAM("MODE +k "));
		_server->log("Applying mode K: Removing key", RED);
		channel->setKey("");
	}
}

void ModeHandler::applyModeT(Client *client, Channel* channel, bool adding)
{
	std::string modeChange;
	bool isAlreadySet;

	modeChange = adding ? "+t" : "-t";
	isAlreadySet = channel->getTopicProtection() == adding;
	if (!isAlreadySet)
	{
		_server->log("Applying mode T: " + std::string(adding ? "Setting topic protection" : "Removing topic protection"), GREEN);
		channel->setTopicProtection(adding);
		// Inform the client and channel about the mode change
		channel->broadcast(MODEACCEPTMESSAGE(client, channel->getName(), modeChange, ""), NULL, _server);
	}
}

void ModeHandler::applyModeO(Client* client, Channel* channel, bool adding, const std::string& argument)
{
	Client* targetClient = _server->getClientByName(argument);
	if (!targetClient)
	{
		_server->sendToClient(client->getFd(), ERR_NOSUCHNICK(client, argument));
		_server->log("Mode o error: No such client " + argument, RED);
		return;
	}

	if (!channel->hasClient(targetClient))
	{
		_server->sendToClient(client->getFd(), ERR_USERNOTINCHANNEL(client, argument, channel->getName()));
		_server->log("Mode o error: Client " + argument + " not in channel " + channel->getName(), RED);
		return;
	}

	if (adding)
	{
		_server->log("Applying mode o: Adding operator " + argument, GREEN);
		channel->addOperator(targetClient);
		std::string message = ":" + client->getNickname() + " MODE " + channel->getName() + " +o " + targetClient->getNickname() + "\r\n";
		channel->broadcast(message, NULL, _server);
	}
	else
	{
		_server->log("Applying mode o: Removing operator " + argument, RED);
		channel->removeOperator(targetClient);
		std::string message = ":" + client->getNickname() + " MODE " + channel->getName() + " -o " + targetClient->getNickname() + "\r\n";
		channel->broadcast(message, NULL, _server);
	}
}

bool ModeHandler::needsArgument(char mode)
{
	return mode == 'k' || mode == 'l' || mode == 'o';
}
