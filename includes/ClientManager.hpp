/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcheca <jcheca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 18:30:07 by jcheca            #+#    #+#             */
/*   Updated: 2024/06/12 11:03:49 by jcheca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTMANAGER_HPP
#define CLIENTMANAGER_HPP

#include "Client.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "CommandHandler.hpp"
#include "RPL.hpp"
#include "Utils.hpp"
#include "BotFilter.hpp"

#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <fcntl.h>
#include <iostream>
#include <netdb.h>
#include <arpa/inet.h>
#include <fstream>
#include <map>
#include <vector>

class Server;
class Client;
class BotFilter;

class ClientManager
{
	public:
		ClientManager(Server *server);
		~ClientManager();
		void acceptClient();
		void handleClientNext(int client_fd, char * buffer, int bytes_received);
		void handleClient(int client_fd);
		void removeClient(int client_fd);

	private:
		Server *_server;
		BotFilter *_botFilter;
};

#endif
