#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <poll.h>
#include <sstream>
#include <cstring>
#include <vector>
#include <map>
#include <iostream>

#include "RPL.hpp"

#define MAX_CLIENTS 10
#define MAX_CHANNELS 10
#define MAX_CHANNEL_NAME 50

#include "Client.hpp"

std::vector<std::string> split(const std::string& input, const std::string& delimiters) {
    std::vector<std::string> result;
    size_t startPos = 0;
    size_t foundPos = input.find_first_of(delimiters, startPos);

    while (foundPos != std::string::npos) {
        if (foundPos != startPos) {
            result.push_back(input.substr(startPos, foundPos - startPos));
        }
        startPos = foundPos + 1;
        foundPos = input.find_first_of(delimiters, startPos);
    }

    if (startPos < input.length()) {
        result.push_back(input.substr(startPos));
    }

    return result;
}

int main(int argc, char * argv[])
{
	const char* port = "8000";
	std::string pass = argv[2];
	if (argc != 3)
		return(-1);
	else
	{
		port = argv[1];
		pass = argv[2];
	}
	struct addrinfo hints, *res;
	char buf[512];

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	
	if (getaddrinfo(NULL, port, &hints, &res) != 0)
	{
		perror("getaddrinfo");
		exit(EXIT_FAILURE);
	}
	int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sockfd < 0) {
		perror("socket");
		exit(EXIT_FAILURE);
	}
	if (bind(sockfd, res->ai_addr, res->ai_addrlen) < 0)
	{
		perror("bind");
		exit(EXIT_FAILURE);
	}
	if (listen(sockfd, MAX_CLIENTS) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}

	std::cout << "Server listening on port " << port << "..." << std::endl;
	struct pollfd fds[MAX_CLIENTS];
	fds[0].fd = sockfd;
	fds[0].events = POLLIN;
	int on = 5;

	std::vector<int> clientSocketsVector;
	std::map<int,Client> ClientMap;

	while (on > 1)
	{
		std::cout << "_*_" << std::endl;

		int ret = poll(fds, clientSocketsVector.size() + 1, -1);
	

		std::cout << "*__*__*__*" << std::endl;
	
		for (unsigned int i = 0; i < clientSocketsVector.size(); ++i)
		{
			std::cout << "** " << i << " " << ClientMap[i].getfd() << " " << ClientMap[i].getname() << " **" << std::endl;

			if (fds[i + 1].revents & POLLIN)
			{
				memset(buf, '\0', sizeof(buf));
				int nbytes = recv(clientSocketsVector[i], buf, sizeof(buf), 0);
				std::cout << i << buf << std::endl;
				if (nbytes <= 0)
				{
					if (nbytes == 0)
					{
						// Client disconnected
						std::cout << "Client disconnected." << std::endl;
						close(clientSocketsVector[i]);
						clientSocketsVector.erase(clientSocketsVector.begin() + i);
						fds[i + 1].fd = -1; // Mark the file descriptor as closed
					}
					else
					{
						perror("recv");
					}
				}
				else
				{
					// Handle received data
					std::cout << "Received data from client: " << buf << std::endl;

					if (strncmp(buf, "PASS", 4) == 0)
					{
						std::vector<std::string> tokens = split(buf, " \n\r\t");
						std::cout << "PASS   :   " << tokens[1] << std::endl;

						if (strncmp(pass.c_str(), tokens[1].c_str(), sizeof(pass)) == 0) 
						{
							std::cout << "pass correct. " << std::endl;
							//send(fds[i+1].fd, "Welcome to the IRC server!\r\n", 28, 0);
							;
						}
						else
						{
							std::cout << "pass incorrect. " << std::endl;
							send(fds[i+1].fd, "Bad Password!\n", 14, 0);
							close(clientSocketsVector[i]);
							clientSocketsVector.erase(clientSocketsVector.begin() + i);
							fds[i + 1].fd = -1;
						}	
					}
					if (strncmp(buf, "CAP", 3) == 0)
					{
						std::vector<std::string> tokens = split(buf, " \n\r\t");
						if (tokens.size() != 3)
						{
							std::cout << "CAP PASS   :   " << tokens[3] << tokens[4] << std::endl;
							if (strncmp("PASS", tokens[3].c_str(), 4) == 0)
							{
								if (strncmp(pass.c_str(), tokens[4].c_str(), sizeof(pass)) == 0) 
								{
									std::cout << "pass correct. " << std::endl;
									//send(fds[i+1].fd, "Welcome to the IRC server!\r\n", 28, 0);
									;
								}
								else
								{
									std::cout << "pass incorrect. " << std::endl;
									send(fds[i+1].fd, "Bad Password!\n", 14, 0);
									close(clientSocketsVector[i]);
									clientSocketsVector.erase(clientSocketsVector.begin() + i);
									fds[i + 1].fd = -1;
								}
							}
						}
					}
					if (strncmp(buf, "QUIT", 4) == 0)
						clientSocketsVector.erase(clientSocketsVector.begin() + (ret - 1));
					if (strncmp(buf, "NICK", 4) == 0)
					{
						std::vector<std::string> tokens = split(buf, " \n\r\t");
						for (size_t t = 0; t < tokens.size(); ++t) {
							std::cout << t << tokens[t] << std::endl;
						}
						std::cout << tokens.size() << std::endl;
						ClientMap[i].setname(tokens[1]);
						ClientMap[i].setuser(tokens[3]);
						if (ClientMap[i].getlogged() == false)
						{
							std::cout << "getname = " << ClientMap[i].getname() << std::endl;
							std::cout << "getuser = " << ClientMap[i].getuser() << std::endl;
							ClientMap[i].setlogged();
							
							send(ClientMap[i].getfd(),RPL_WELCOME(ClientMap[i]).c_str(), RPL_WELCOME(ClientMap[i]).size(), 0);
							std::cout << "88 " << i << " " << ClientMap[i].getfd() << " " << ClientMap[i].getname() << " 99" << std::endl;

						}
						/*
						ss << ": 001 " << user.getuser() << " :Welcome to the 42 Network, " << user.getname() << "\r\n";
						ss << ": 002 " << user.getuser() << " :Your host is ft_irc, running version 42.0" << "\r\n";
						ss << ": 003 " << user.getuser() << " :This server was created 42 42 2042" << "\r\n";
						*/

					}
					if (strncmp(buf, "JOIN", 4) == 0)
					{
					
					}
					if (strncmp(buf, "stop", 4) == 0)
					{
						break ;
					}
				}
			}
		}
		if (fds[0].revents & POLLIN)
		{
			int new_fd = accept(sockfd, NULL, NULL);
			if (new_fd < 0)
			{
				perror("accept");
				continue;
			}
			std::cout << "New connection accepted." << std::endl;
			fds[clientSocketsVector.size() + 1].fd = new_fd;
			fds[clientSocketsVector.size() + 1].events = POLLIN;
			ClientMap[clientSocketsVector.size()] = Client(new_fd, "anon", "anon");
			std::cout << "accept = " << new_fd << " " << ClientMap[clientSocketsVector.size()].getfd() << std::endl;
			clientSocketsVector.push_back(new_fd);
		}
		std::cout << "***********************************" << std::endl;
	}
	close(sockfd);
	//freeaddrinfo(res);
	return 0;
}
