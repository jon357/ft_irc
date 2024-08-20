/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BotFilter.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcheca <jcheca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 11:44:44 by jcheca            #+#    #+#             */
/*   Updated: 2024/06/12 11:02:00 by jcheca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOTFILTER_HPP
#define BOTFILTER_HPP

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

class Server;
class Client;
class Channel;
class BotFilter
{
public:
	BotFilter(Server* server);
	~BotFilter();
	void loadBadWords(const std::string& fileName);
	bool checkMessage(Client* client, Channel* channel, const std::string& message);
	void warnClient(Client* client, Channel* channel);
	void warnClientDirect(Client* client);
	void kickClient(Client* client, Channel* channel);

private:
	bool containsBadWords(const std::string& message);

	Server* _server;
	std::vector<std::string> _badWords;
	std::map<int, int> _warnings;
};

#endif
