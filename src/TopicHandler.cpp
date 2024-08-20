/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TopicHandler.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcheca <jcheca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 17:04:58 by jcheca            #+#    #+#             */
/*   Updated: 2024/06/12 11:12:14 by jcheca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TopicHandler.hpp"

TopicHandler::TopicHandler(Server* server) : _server(server) {}

TopicHandler::~TopicHandler() {}

void TopicHandler::handleTopicCommand(Client* client, const std::string& command)
{
	_server->log("Received TOPIC command: " + command, "\033[1;34m");
	std::vector<std::string> tokens = split(command, " \n\r\t");

	if (tokens.size() < 2)
	{
		_server->sendToClient(client->getFd(), ERR_NEEDMOREPARAMS(client, NULL, "TOPIC"));
		_server->log("TOPIC command error: Need more parameters", RED);
		return;
	}

	std::string channelName = tokens[1];
	Channel* channel = _server->getChannelByName(channelName);
	if (!channel)
	{
		_server->sendToClient(client->getFd(), ERR_NOSUCHCHANNEL(client, channelName));
		_server->log("TOPIC command error: No such channel " + channelName, RED);
		return;
	}

	if (!channel->hasClient(client))
	{
		_server->sendToClient(client->getFd(), ERR_NOTONCHANNEL(client, channelName));
		_server->log("TOPIC command error: Client not on channel " + channelName, RED);
		return;
	}

	if (tokens.size() == 2)
	{
		viewTopic(client, channel);
	}
	else
	{
		size_t topicStart = command.find(" :");
		std::string topic = (topicStart != std::string::npos) ? command.substr(topicStart + 2) : "";
		changeTopic(client, channel, topic);
	}
}

void TopicHandler::viewTopic(Client* client, Channel* channel)
{
	if (channel->getTopic().empty())
	{
		_server->sendToClient(client->getFd(), RPL_NOTOPIC(client, channel->getName()));
		_server->log("No topic set for channel " + channel->getName(), GREEN);
	}
	else
	{
		_server->sendToClient(client->getFd(), RPL_TOPIC(client, channel->getName(), channel->getTopic()));
		_server->sendToClient(client->getFd(), RPL_TOPICWHOTIME(client, channel->getName(), channel->getTopicSetter(), channel->getTopicTime()));
		_server->log("Sent topic info for channel " + channel->getName(), GREEN);
	}
}

void TopicHandler::changeTopic(Client* client, Channel* channel, const std::string& topic)
{
	if (channel->getTopicProtection() && !channel->isOperator(client))
	{
		_server->sendToClient(client->getFd(), ERR_CHANOPRIVSNEEDED(client, channel->getName()));
		_server->log("TOPIC command error: Channel operator privileges needed for " + channel->getName(), RED);
		return;
	}

	if (topic == channel->getTopic())
	{
		_server->sendToClient(client->getFd(), RPL_TOPIC(client, channel->getName(), channel->getTopic()));
		_server->log("TOPIC unchanged for channel " + channel->getName(), GREEN);
		return;
	}

	channel->setTopic(topic, client->getNickname());
	std::vector<Client*> clients = channel->getClients();
	for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		_server->sendToClient((*it)->getFd(), ":" + client->getNickname() + " TOPIC " + channel->getName() + " :" + topic + "\r\n");
	}

	_server->log("Changed topic for channel " + channel->getName() + " to: " + topic, GREEN);
}
