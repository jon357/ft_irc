/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcheca <jcheca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 12:16:41 by jcheca            #+#    #+#             */
/*   Updated: 2024/05/15 12:49:07 by jcheca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Utils.hpp"
#include <iostream>
#include <cstdlib>

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		std::cerr << RED << "Usage: " << argv[0] << " <port> <password>" << RESET << std::endl;
		return EXIT_FAILURE;
	}

	int port = std::atoi(argv[1]);
	std::string password = argv[2];

	Server server(port, password);
	server.run();

	return EXIT_SUCCESS;
}
