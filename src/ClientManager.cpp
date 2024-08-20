/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcheca <jcheca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 18:32:23 by jcheca            #+#    #+#             */
/*   Updated: 2024/06/12 15:01:26 by jcheca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientManager.hpp"

ClientManager::ClientManager(Server *server)
	: _server(server)
{
	_botFilter = new BotFilter(server);
	_botFilter->loadBadWords("badwords.txt");
}

ClientManager::~ClientManager()
{
	delete _botFilter;
}

void ClientManager::acceptClient()
{
	int client_fd = accept(_server->_server_fd, NULL, NULL);
	if (client_fd < 0)
	{
		_server->log("Failed to accept client.", RED);
		return;
	}

	Client *newClient = new Client(client_fd, "", "", "", "", "");
	_server->_clients[client_fd] = newClient;
	struct pollfd client_pollfd;
	client_pollfd.fd = client_fd;
	client_pollfd.events = POLLIN;
	_server->_poll_fds.push_back(client_pollfd);

	std::stringstream ss;
	ss << "Client attempting connection: " << client_fd;
	_server->log(ss.str(), YELLOW);
}

void ClientManager::handleClient(int client_fd)
{
	Client* client = _server->getClients()[client_fd];
	std::memset(client->buffer, 0, sizeof(client->buffer));
	int bytes_received = recv(client_fd, client->buffer, sizeof(client->buffer), 0);

	if (bytes_received <= 0)
	{
		std::ostringstream oss;
		oss << "Client disconnected: " << client_fd;
		_server->log(oss.str(), RED);
		removeClient(client_fd);
		return;
	}

	std::cout << std::string(client->buffer).size() << " client->buffer " << std::string(client->buffer).find('\n') << std::endl;

	for (size_t i = 0; client->buffer[i]; i++)
	{
		std::cout << client->buffer[i] << " .. ";
	}
	std::cout << std::endl;

	if (std::string(client->buffer).find('\n') != std::string::npos)
	{
		strcat(client->buffer2, client->buffer);

		bool messageAllowed = true;
		std::set<std::string>::const_iterator it;
		for (it = client->getChannels().begin(); it != client->getChannels().end(); ++it)
		{
			const std::string& channelName = *it;
			Channel* channel = _server->getChannelByName(channelName);
			if (channel && !_botFilter->checkMessage(client, channel, std::string(client->buffer2)))
			{
				messageAllowed = false;
				break;
			}
		}

		if (messageAllowed)
		{
			handleClientNext(client_fd, client->buffer2, std::string(client->buffer2).size());
		}

		std::memset(client->buffer2, 0, std::string(client->buffer2).size());
	}
	else
	{
		strcat(client->buffer2, client->buffer);
		for (size_t i = 0; client->buffer2[i]; i++)
		{
			std::cout << client->buffer2[i] << " . ";
		}
	}
	std::cout << std::endl;
}

void ClientManager::handleClientNext(int client_fd, char * buffer, int bytes_received)
{
	std::string message(buffer, bytes_received);
	std::ostringstream oss;
	oss << "Received from client " << client_fd << ": " << message;
	_server->log(oss.str(), BLUE);

	Client* client = _server->getClients()[client_fd];
	if (!client)
	{
		std::ostringstream oss;
		oss << "Client not found for fd: " << client_fd;
		_server->log(oss.str(), RED);
		return;
	}

	std::istringstream message_stream(message);
	std::string line;

	while (std::getline(message_stream, line))
	{
		line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
		line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());

		if (!line.empty())
		{
			std::ostringstream oss;
			oss << "Processing command from client " << client_fd << ": " << line;
			_server->log(oss.str(), BLUE);
			_server->_commandHandler->handleCommand(client, line);
		}
	}
}

void ClientManager::removeClient(int clientFd)
{
	Client* client = _server->_clients[clientFd];
	if (client)
	{
		_server->log("Removing client: " + client->getNickname(), YELLOW);

		std::map<std::string, Channel*>::iterator it = _server->_channels.begin();
		while (it != _server->_channels.end())
		{
			it->second->removeClient(client);
			if (it->second->isEmpty())
			{
				delete it->second;
				std::map<std::string, Channel*>::iterator it_to_delete = it++;
				_server->_channels.erase(it_to_delete);
			}
			else
			{
				++it;
			}
		}
		delete client;
		_server->_clients.erase(clientFd);
	}

	std::vector<struct pollfd>::iterator it_poll = _server->_poll_fds.begin();
	while (it_poll != _server->_poll_fds.end())
	{
		if (it_poll->fd == clientFd)
		{
			_server->_poll_fds.erase(it_poll);
			break;
		}
		++it_poll;
	}

	std::stringstream ss;
	ss << "Client disconnected: " << clientFd;
	_server->log(ss.str(), YELLOW);
}
