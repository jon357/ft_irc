/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AdditionalCommands.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcheca <jcheca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 15:27:29 by jcheca            #+#    #+#             */
/*   Updated: 2024/06/12 11:01:35 by jcheca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AdditionalCommands.hpp"

AdditionalCommands::AdditionalCommands(Server *server) : _server(server) {}

std::string toUpperCase(const std::string &str)
{
	std::string upperStr = str;
	std::transform(upperStr.begin(), upperStr.end(), upperStr.begin(), ::toupper);
	return upperStr;
}

AdditionalCommands::~AdditionalCommands()
{}

void AdditionalCommands::processCommand(Client *client, const std::string &command)
{
	std::istringstream iss(command);
	std::string cmd;
	iss >> cmd;
	std::string upperCommand = toUpperCase(cmd) + command.substr(cmd.length());

	// Log the received command
	_server->log("Received command: " + command, BLUE);
	_server->log("Converted command to uppercase: " + upperCommand, BLUE);

	if (upperCommand.find("PART") == 0)
	{
		_server->log("Handling PART command", GREEN);
		handlePartCommand(_server, client, command);
	}
	else if (upperCommand.find("PRIVMSG") == 0)
	{
		_server->log("Handling PRIVMSG command", GREEN);
		handlePrivmsgCommand(_server, client, command);
	}
	else if (upperCommand.find("WHO") == 0)
	{
		_server->log("Handling WHO command", GREEN);
		WhoHandler whoHandler(_server);
		whoHandler.handleWhoCommand(client, command);
	}
	else if (upperCommand.find("WHOIS") == 0)
	{
		_server->log("Handling WHOIS command", GREEN);
		WhoHandler whoHandler(_server);
		whoHandler.handleWhoisCommand(client, command);
	}
	else if (upperCommand.find("INVITE") == 0)
	{
		_server->log("Handling INVITE command", GREEN);
		InviteHandler inviteHandler(_server);
		inviteHandler.handleInviteCommand(client, command);
	}
	else if (upperCommand.find("TOPIC") == 0)
	{
		_server->log("Handling TOPIC command", GREEN);
		TopicHandler topichandler(_server);
		topichandler.handleTopicCommand(client, command);
	}
	else if (upperCommand.find("LIST") == 0)
	{
		_server->log("Handling LIST command", GREEN);
		broadcastChannelList(client, _server);
	}
	else if (upperCommand.find("KICK") == 0)
	{
		_server->log("Handling KICK command", GREEN);
		KickHandler kickHandler(_server);
		kickHandler.handleKickCommand(client, command);
	}
	else
	{
		_server->log("Unknown command received: " + command, RED);
		_server->sendToClient(client->getFd(), ERR_UNKNOWNCOMMAND(client, command));
		_server->log("Sent unknown command error to client: " + client->getNickname(), MAGENTA);
	}
}

void AdditionalCommands::broadcastChannelList(Client *client, Server *server)
{
	std::map<std::string, Channel *> &channels = server->getChannels();
	for (std::map<std::string, Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		server->sendToClient(client->getFd(), RPL_LIST(client, it->first, it->second->getClients().size(), it->second->getTopic()));
	}
	server->sendToClient(client->getFd(), RPL_LISTEND(client));
}

void AdditionalCommands::handlePartCommand(Server *server, Client *client, const std::string &command)
{
	std::istringstream iss(command);
	std::string cmd, channelNames;
	iss >> cmd >> channelNames;

	if (channelNames.empty())
	{
		server->sendToClient(client->getFd(), ERR_NEEDMOREPARAMS(client, NULL, "PART"));
		return;
	}

	std::vector<std::string> channels = split(channelNames, ",");
	std::map<std::string, Channel *> &channelMap = server->getChannels();

	for (size_t i = 0; i < channels.size(); ++i)
	{
		std::string &channelName = channels[i];
		if (channelMap.find(channelName) == channelMap.end())
		{
			server->sendToClient(client->getFd(), ERR_NOSUCHCHANNEL(client, channelName));
			continue;
		}

		Channel *channel = channelMap[channelName];
		if (!channel->hasClient(client)) {
			server->sendToClient(client->getFd(), ERR_NOTONCHANNEL(client, channelName));
			continue;
		}

		channel->removeClient(client);

		std::ostringstream partMsg;
		partMsg << ":" << client->getNickname() << " PART " << channelName << " Leaving" << "\r\n";
		server->sendToClient(client->getFd(), partMsg.str());

		channel->broadcast(partMsg.str(), client, server);

		if (channel->isEmpty())
		{
			delete channel;
			channelMap.erase(channelName);
		}

		server->log("Client " + client->getNickname() + " left channel " + channelName, MAGENTA);
	}
}

void AdditionalCommands::handlePrivmsgCommand(Server *server, Client *client, const std::string &command)
{
	std::istringstream iss(command);
	std::string cmd, target, message;
	iss >> cmd >> target;
	getline(iss, message);

	_server->log("Handling PRIVMSG command from: " + client->getNickname(), GREEN);

	// Enlever le ':' initial dans le message si présent
	if (!message.empty() && message[0] == ':')
		message = message.substr(1);

	_server->log("Target: " + target + ", Message: " + message, GREEN);

	if (target.empty())
	{
		_server->log("No recipient specified for PRIVMSG", RED);
		server->sendToClient(client->getFd(), ERR_NORECIPIENT(client, "PRIVMSG"));
		return;
	}

	if (message.empty())
	{
		_server->log("No text to send for PRIVMSG", RED);
		server->sendToClient(client->getFd(), ERR_NOTEXTTOSEND(client));
		return;
	}

	// Récupérer les canaux du serveur
	std::map<std::string, Channel *> &channels = server->getChannels();

	// Si la cible est un canal
	if (channels.find(target) != channels.end())
	{
		Channel *channel = channels[target];

		_server->log("Target is a channel: " + target, GREEN);

		// Vérifier les conditions spéciales du canal (ex: ban, modération)
		if (channel->isBanned(client))
		{
			_server->log("Client is banned from channel: " + target, RED);
			server->sendToClient(client->getFd(), ERR_CANNOTSENDTOCHAN(client, target));
			return;
		}

		// Check the message with the bot filter before sending it
		if (!server->getBotFilter()->checkMessage(client, channel, message))
		{
			_server->log("Message contains inappropriate content and was filtered", RED);
			// If the message is inappropriate, the bot filter will handle warnings or kicks
			return;
		}

		std::vector<Client *> channelClients = channel->getClients();

		for (size_t i = 0; i < channelClients.size(); ++i)
		{
			// Envoyer le message à tous les clients du canal sauf l'émetteur
			if (channelClients[i] != client)
			{
				std::stringstream privMsg;
				privMsg << ":" << client->getNickname() << " PRIVMSG " << target << message << "\r\n";
				server->sendToClient(channelClients[i]->getFd(), privMsg.str());
				_server->log("Sent PRIVMSG to " + channelClients[i]->getNickname(), GREEN);
			}
		}
	}
	// Si la cible est un utilisateur
	else
	{
		_server->log("Target is a user: " + target, GREEN);

		Client *targetClient = server->getClientByName(target); // Utiliser getClientByName pour trouver le client par nom

		if (targetClient)
		{
			if (!server->getBotFilter()->checkMessage(client, NULL, message))
			{
				_server->log("Message contains inappropriate content and was filtered", RED);
				// If the message is inappropriate, the bot filter will handle warnings or kicks
				return;
			}

			std::stringstream privMsg;
			privMsg << ":" << client->getNickname() << " PRIVMSG " << target << message << "\r\n";
			server->sendToClient(targetClient->getFd(), privMsg.str());
			_server->log("Sent PRIVMSG to " + targetClient->getNickname(), GREEN);

			// Check the message with the bot filter before sending it
			if (targetClient->isAway())
			{
				server->sendToClient(client->getFd(), RPL_AWAY(client, targetClient->getNickname(), targetClient->getAwayMessage()));
				_server->log(targetClient->getNickname() + " is away, sent AWAY reply", GREEN);
			}
		}
		else
		{
			_server->log("No such nick/channel: " + target, RED);
			// Si la cible n'est ni un canal ni un utilisateur existant, envoyer un message d'erreur
			server->sendToClient(client->getFd(), ERR_NOSUCHNICK(client, target));
		}
	}
}
