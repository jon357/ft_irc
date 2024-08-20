/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcheca <jcheca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 12:15:13 by jcheca            #+#    #+#             */
/*   Updated: 2024/06/12 12:58:02 by jcheca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Utils.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "ClientManager.hpp"
#include "CommandHandler.hpp"
#include "AdditionalCommands.hpp"
#include "RPL.hpp"
#include "ModeHandler.hpp"
#include "TopicHandler.hpp"
#include "BotFilter.hpp"
#include "Join.hpp"
#include "Welcome.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <string>
#include <vector>
#include <map>
#include <poll.h>
#include <csignal>


class Client;
class Channel;
class ClientManager;
class CommandHandler;
class AdditionalCommands;
class ModeHandler;
class TopicHandler;
class BotFilter;
class JoinHandler;
class WelcomeHandler;

class Server
{
	public:
		Server(int port, const std::string &password);
		~Server();
		void run();
		void log(const std::string &message, const std::string &color = "\033[0m");
		void sendToClient(int client_fd, const std::string &message);

		std::map<std::string, Channel *> &getChannels();
		std::map<int, Client *> &getClients();
		const std::string &getPassword() const;
		void broadcast(const std::string &message);
		Client* getClientByName(const std::string &name);
		Channel* getChannelByName(const std::string &name);
		void disconnectClient(int clientFd);
		bool MatchFd(const pollfd& pfd, int clientFd);
		void removePollFd(int clientFd);
		BotFilter* getBotFilter() const;

	protected:
		int _server_fd;
		int _port;
		std::string _password;
		std::map<int, Client *> _clients;
		std::map<std::string, Channel *> _channels;
		std::vector<struct pollfd> _poll_fds;
		ClientManager *_clientManager;
		CommandHandler *_commandHandler;
		ModeHandler *_modeHandler;
		TopicHandler *_topicHandler;
		BotFilter *_botFilter;
		AdditionalCommands *_additionalCommands;
		JoinHandler *_joinHandler;
		WelcomeHandler *_welcomeHandler;
		static Server *instance;
		static void signalHandler(int signum);

		friend class ClientManager;
		friend class CommandHandler;
		friend class ModeHandler;
		friend class TopicHandler;

	private:
		void initServer();
		void handleServerCommands();
		bool bool_exit;
};

#endif
