/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Welcome.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcheca <jcheca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 19:53:17 by jcheca            #+#    #+#             */
/*   Updated: 2024/06/12 11:13:27 by jcheca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WELCOME_HPP
#define WELCOME_HPP

#include "Client.hpp"
#include "Server.hpp"
#include "Utils.hpp"

#include <string>

class WelcomeHandler
{
	public:
		WelcomeHandler(Server *server);
		~WelcomeHandler();
		void sendWelcomeMessages(Client *client, Server *server);
		void sendMotd(Client *client, Server *server);

	private:
		Server *_server;
};

#endif
