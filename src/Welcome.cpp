/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Welcome.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcheca <jcheca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 17:53:52 by jcheca            #+#    #+#             */
/*   Updated: 2024/06/12 11:14:28 by jcheca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Welcome.hpp"

WelcomeHandler::WelcomeHandler(Server *server) : _server(server) {}

WelcomeHandler::~WelcomeHandler() {}

void WelcomeHandler::sendWelcomeMessages(Client *client, Server *server)
{
	server->sendToClient(client->getFd(), RPL_WELCOME(client));
	server->sendToClient(client->getFd(), RPL_YOURHOST(client));
	server->sendToClient(client->getFd(), RPL_CREATED(client));
	server->sendToClient(client->getFd(), RPL_MYINFO(client));
	server->sendToClient(client->getFd(), RPL_ISUPPORT(client, "i/t/k/o/l"));

	sendMotd(client, server);
}

void WelcomeHandler::sendMotd(Client *client, Server *server)
{
	std::ifstream motdFile("motd.txt");
	if (motdFile.is_open())
	{
		std::string line;
		server->sendToClient(client->getFd(), RPL_MOTDSTART(client));
		while (std::getline(motdFile, line))
		{
			server->sendToClient(client->getFd(), RPL_MOTD(client, line));
		}
		server->sendToClient(client->getFd(), RPL_ENDOFMOTD(client));
		motdFile.close();
	}
	else
	{
		server->sendToClient(client->getFd(), ERR_NOMOTD(client));
	}
}
