/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcheca <jcheca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 12:15:42 by jcheca            #+#    #+#             */
/*   Updated: 2024/06/12 12:58:08 by jcheca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <set>
#include <cstring> 

class Client
{
public:
	Client(int fd, const std::string &nickname, const std::string &user, const std::string &host, const std::string &password, const std::string &realname);
	~Client();

	int getFd() const;
	const std::string &getNickname() const;
	void setNickname(const std::string &nickname);
	const std::string &getUser() const;
	void setUser(const std::string &user);
	const std::string &getHost() const;
	void setHost(const std::string &host);
	const std::string &getPassword() const;
	void setPassword(const std::string &password);
	const std::string &getRealName() const;
	void setRealName(const std::string &realname);
	bool isAuthenticated() const;
	void authenticate();
	bool isOperator() const;
	void setOperator(bool isOperator);

	// Ajout des méthodes pour la gestion du statut "away"
	bool isAway() const;
	const std::string &getAwayMessage() const;
	void setAwayMessage(const std::string &message);
	void setAway(bool away);
	std::string getKey() const;
	void setkey(const std::string &key);
	char buffer[1024];
	char buffer2[1024];
	void joinChannel(const std::string &channel);
	void leaveChannel(const std::string &channel);
	const std::set<std::string> &getChannels() const;

private:
	int _fd;
	std::string _nickname;
	std::string _user;
	std::string _host;
	std::string _password;
	std::string _realname;
	bool _authenticated;
	bool _operator;
	bool _away;
	std::string _awayMessage;
	std::string _key;
	std::set<std::string> _channels;
};

#endif
