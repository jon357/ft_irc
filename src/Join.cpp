/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcheca <jcheca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 19:51:31 by jcheca            #+#    #+#             */
/*   Updated: 2024/05/21 20:13:30 by jcheca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Join.hpp"

JoinHandler::JoinHandler(Server *server) : _server(server)
{}

JoinHandler::~JoinHandler() {}

void JoinHandler::handleJoinCommand(Client* client, const std::string& params, Server* server)
{
	std::map<std::string, Channel*>& channels = server->getChannels();
	Channel* channel = NULL;

	server->log("Received JOIN command with params: " + params, RED);

	// Split params into channel names and keys
	std::vector<std::string> parts = split(params, " ");
	if (parts.empty())
	{
		server->sendToClient(client->getFd(), ERR_NEEDMOREPARAMS(client, NULL, "JOIN"));
		return;
	}

	std::string channelNames = parts[0];
	std::string keys = (parts.size() > 1) ? parts[1] : "";

	std::vector<std::string> channelList = split(channelNames, ",");
	std::vector<std::string> keyList = split(keys, ",");

	for (size_t i = 0; i < channelList.size(); ++i)
	{
		const std::string& channelName = channelList[i];
		server->log("Processing channel: " + channelName, RED);

		if (channelName.empty())
		{
			server->sendToClient(client->getFd(), ERR_NEEDMOREPARAMS(client, NULL, "JOIN"));
			return;
		}

		if (channelName[0] == '0' && channelName.size() == 1)
		{
			std::cout << "leave all -----------" << std::endl;
			std::map<std::string, Channel *> & channels = server->getChannels();
			for (std::map<std::string, Channel *>::iterator it = channels.begin(); it != channels.end();)
			{
				// std::cout << it->second->getName() << std::endl;
				if (it->second->hasClient(client))
				{
					std::ostringstream partMsg;
					partMsg << ":" << client->getNickname() << " PART " << it->first << " Leaving" << "\r\n";
					server->sendToClient(client->getFd(), partMsg.str());

					it->second->broadcast(partMsg.str(), client, server);
					it->second->removeClient(client);
					if (it->second->isEmpty())
					{
						std::map<std::string, Channel*>::iterator itcopy = it;
						itcopy++;
						delete it->second;
						channels.erase(it->first);
						it = itcopy;
					}

				}
				else
				{
					++it;
				}
			}
			// Leave all channels
			return;
		}

		if (channelName[0] != '#' && channelName[0] != '&')
		{
			server->sendToClient(client->getFd(), ERR_NOSUCHCHANNEL(client, channelName));
			return;
		}

		if (channels.find(channelName) != channels.end())
		{
			channel = channels[channelName];

			if (channel->isBanned(client))
			{
				server->sendToClient(client->getFd(), ERR_BANNEDFROMCHAN(client, channelName));
				return;
			}
		}

		if (channels.find(channelName) == channels.end())
		{
			channel = new Channel(channelName);
			channels[channelName] = channel;
			channel->addOperator(client); // Set client as operator for new channel
		}
		else
		{
			channel = channels[channelName];
		}

		if (channel == NULL)
		{
			server->log("Channel is NULL after search or creation. This should not happen.", RED);
			return;
		}

		if (channel->isFull())
		{
			server->sendToClient(client->getFd(), ERR_CHANNELISFULL(client, channelName));
			return;
		}

		if (channel->isInviteOnly() && !channel->isInvited(client))
		{
			server->sendToClient(client->getFd(), ERR_INVITEONLYCHAN(client, channelName));
			return;
		}

		// Extract the key for this channel, if provided
		std::string key = (i < keyList.size()) ? keyList[i] : "";
		server->log("Using key: " + key + " for channel: " + channelName, RED);

		if (!channel->checkKey(key))
		{
			server->sendToClient(client->getFd(), ERR_BADCHANNELKEY(client, channelName));
			return;
		}

		channel->addClient(client);
		sendJoinSuccess(client, channel, server);
	}
}

void JoinHandler::sendJoinSuccess(Client* client, Channel* channel, Server* server)
{
	// Construct the join message and notify all clients in the channel
	std::string joinMsg = ":" + client->getNickname() + " JOIN " + channel->getName() + "\r\n";
	server->sendToClient(client->getFd(), joinMsg);
	channel->broadcast(joinMsg, client, server);

	// Send topic information to the new client
	if (channel->getTopic().empty())
	{
		server->sendToClient(client->getFd(), RPL_NOTOPIC(client, channel->getName()));
	}
	else
	{
		server->sendToClient(client->getFd(), RPL_TOPIC(client, channel->getName(), channel->getTopic()));
	}

	// Send channel mode and creation time
	std::string channelModes = RPL_CHANNELMODEIS(client->getFd(), channel->getName(), channel->getModes());
	std::string channelCreationTime = RPL_CREATIONTIME(client, channel->getName(), channel->getTopicTime());

	server->sendToClient(client->getFd(), channelModes + channelCreationTime);

	// Get the users list and send the names reply
	std::string usersList = getUsersList(channel);
	server->sendToClient(client->getFd(), usersList);
}


std::string JoinHandler::getUsersList(Channel* channel)
{
	std::vector<Client*> clients = channel->getClients();
	std::string users;
	for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (channel->isOperator((*it)))
			users += "@";
		users += (*it)->getNickname() + " ";
	}

	// Remove trailing space
	if (!users.empty() && users[users.size() - 1] == ' ')
	{
		users = users.substr(0, users.size() - 1);
	}

	// Construct the names reply message
	std::ostringstream oss;
	oss << RPL_NAMREPLY(clients[0], channel->getName(), users);
	oss << RPL_ENDOFNAMES(clients[0], channel->getName());
	return oss.str();
}
