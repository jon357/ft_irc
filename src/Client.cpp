/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcheca <jcheca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 12:17:42 by jcheca            #+#    #+#             */
/*   Updated: 2024/06/12 12:51:56 by jcheca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int fd, const std::string &nickname, const std::string &user, const std::string &host, const std::string &password, const std::string &realname)
	: _fd(fd), _nickname(nickname), _user(user), _host(host), _password(password), _realname(realname), _authenticated(false), _operator(false), _away(false)
{
	std::memset(buffer, 0, sizeof(buffer));
	std::memset(buffer2, 0, sizeof(buffer2));
}

Client::~Client()
{}

int Client::getFd() const
{
	return _fd;
}

const std::string &Client::getNickname() const
{
	return _nickname;
}

void Client::setNickname(const std::string &nickname)
{
	_nickname = nickname;
}

const std::string &Client::getUser() const
{
	return _user;
}

void Client::setUser(const std::string &user)
{
	_user = user;
}

const std::string &Client::getHost() const
{
	return _host;
}

void Client::setHost(const std::string &host)
{
	_host = host;
}

const std::string &Client::getPassword() const
{
	return _password;
}

void Client::setPassword(const std::string &password)
{
	_password = password;
}

const std::string &Client::getRealName() const
{
	return _realname;
}

void Client::setRealName(const std::string &realname)
{
	_realname = realname;
}

bool Client::isAuthenticated() const
{
	return _authenticated;
}

void Client::authenticate()
{
	_authenticated = true;
}

bool Client::isOperator() const
{
	return _operator;
}

void Client::setOperator(bool isOperator)
{
	_operator = isOperator;
}

bool Client::isAway() const
{
	return _away;
}

const std::string &Client::getAwayMessage() const
{
	return _awayMessage;
}

void Client::setAwayMessage(const std::string &message)
{
	_awayMessage = message;
}

void Client::setAway(bool away)
{
	_away = away;
}

std::string Client::getKey() const
{
	return _key;
}

void Client::setkey(const std::string &key)
{
	_key = key;
}

void Client::joinChannel(const std::string &channel)
{
	_channels.insert(channel);
}

void Client::leaveChannel(const std::string &channel)
{
	_channels.erase(channel);
}

const std::set<std::string> &Client::getChannels() const
{
	return _channels;
}