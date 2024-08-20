/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KickHandler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcheca <jcheca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 16:59:42 by jcheca            #+#    #+#             */
/*   Updated: 2024/06/12 11:10:14 by jcheca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KICKHANDLER_HPP
#define KICKHANDLER_HPP

#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Utils.hpp"

#include <string>
#include <vector>
#include <sstream>

class KickHandler
{
	public:
		KickHandler(Server* server);
		~KickHandler();
		void handleKickCommand(Client* client, const std::string& command);

	private:
		Server* _server;
};

#endif
