/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KickHandler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcheca <jcheca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 17:00:31 by jcheca            #+#    #+#             */
/*   Updated: 2024/06/12 11:40:37 by jcheca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "KickHandler.hpp"



KickHandler::KickHandler(Server* server) : _server(server) {}

KickHandler::~KickHandler() {}

void KickHandler::handleKickCommand(Client* client, const std::string& command)
{
	std::vector<std::string> tokens = split(command, " ");
	if (tokens.size() < 3)
	{
		_server->sendToClient(client->getFd(), ERR_NEEDMOREPARAMS(client, NULL, "KICK"));
		return;
	}

	std::string channelName = tokens[1];
	std::string targetNickname = tokens[2];
	std::string comment = (tokens.size() > 3) ? tokens[3] : "Kicked by operator";

	Channel* channel = _server->getChannelByName(channelName);
	if (!channel)
	{
		_server->sendToClient(client->getFd(), ERR_NOSUCHCHANNEL(client, channelName));
		return;
	}

	if (!channel->hasClient(client))
	{
		_server->sendToClient(client->getFd(), ERR_NOTONCHANNEL(client, channelName));
		return;
	}

	if (!channel->isOperator(client))
	{
		_server->sendToClient(client->getFd(), ERR_CHANOPRIVSNEEDED(client, channelName));
		return;
	}

	Client* targetClient = _server->getClientByName(targetNickname);
	if (!targetClient)
	{
		_server->sendToClient(client->getFd(), ERR_NOSUCHNICK(client, targetNickname));
		return;
	}

	if (!channel->hasClient(targetClient))
	{
		_server->sendToClient(client->getFd(), ERR_USERNOTINCHANNEL(client, targetNickname, channelName));
		return;
	}

	channel->removeClient(targetClient);
	std::string kickMessage = ":" + client->getNickname() + " KICK " + channelName + " " + targetNickname + " :" + comment + "\r\n";
	channel->broadcast(kickMessage, NULL, _server);
	_server->sendToClient(targetClient->getFd(), kickMessage);
}
