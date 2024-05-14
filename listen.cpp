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
#include <iostream>

#define PORT "8000"
#define MAX_CLIENTS 10
#define MAX_CHANNELS 10
#define MAX_CHANNEL_NAME 50

#include "Client.hpp"

void send_message(int client_socket, const char *message) {
	write(client_socket, message, strlen(message));
}

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

int main()
{
	struct addrinfo hints, *res;
	char buf[512];

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	
	if (getaddrinfo(NULL, PORT, &hints, &res) != 0)
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

	std::cout << "Server listening on port " << PORT << "..." << std::endl;
	struct pollfd fds[MAX_CLIENTS];
	fds[0].fd = sockfd;
	fds[0].events = POLLIN;
	int on = 5;

	std::vector<int> clientSocketsVector;

	

	while (on > 1)
	{
		std::cout << "_*_" << std::endl;

		int ret = poll(fds, clientSocketsVector.size() + 1, -1);
	

		std::cout << "*__*__*__*" << std::endl;
	
		for (unsigned int i = 0; i < clientSocketsVector.size(); ++i)
		{
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
				
					std::string pass = "ff";
					if (strncmp(buf, "PASS", 4) == 0)
					{
						std::vector<std::string> tokens = split(buf, " \n\r\t");
						std::cout << "PASS   :   " << tokens[1] << std::endl;

						if (strncmp(pass.c_str(), tokens[1].c_str(), sizeof(pass)) == 0) 
						{
							std::cout << "pass correct. " << std::endl;
							send(fds[i+1].fd, "Welcome to the IRC server!\r\n", 28, 0);
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
						int j = 0;
						int find = 0;
						while(buf[j])
						{
							if (buf[j]=='\r')
								find = 1;
							j++;
						}
						if (find == 1)
						{

							std::vector<std::string> tokens = split(buf, " \n\r\t");
							for (size_t i = 0; i < tokens.size(); ++i) {
								std::cout << i << tokens[i] << std::endl;
							}
							std::cout << tokens.size() << std::endl;
							std::cout << " __." << std::endl;

							Client user(clientSocketsVector.size(), tokens[1], tokens[3]);
							std::cout << "getname = " << user.getname() << std::endl;
							std::cout << "getuser = " << user.getuser() << std::endl;

							std::stringstream ss;

							// Utilisation de std::cout pour insérer du texte dans ss
							ss << user.getuser() << " :Welcome to the 42 Network, " << user.getname();

							// Convertir le contenu de ss en std::string
							std::string result = ss.str();
							std::cout << result << std::endl;
							send(fds[i].fd, result.c_str(), sizeof(result), 0);
							
							ss << user.getuser() << " :Your host is ft_irc, running version 42.0";
							std::string result = ss.str();
							std::cout << result << std::endl;
							send(fds[i].fd, result.c_str(), sizeof(result), 0);

							ss << user.getuser() << " :This server was created 42 42 2042";
							std::string result = ss.str();
							std::cout << result << std::endl;
							send(fds[i].fd, result.c_str(), sizeof(result), 0);

						}

						/*


						*/
						
					}
					if (strncmp(buf, "JOIN", 4) == 0)
					{
					
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
			clientSocketsVector.push_back(new_fd);
		
			//send(new_fd, "Welcome to the IRC server!\r\n", 28, 0);
		}

		std::cout << "***********************************" << std::endl;

	}
	close(sockfd);
	//freeaddrinfo(res);
	return 0;
}




		/*
		else
		{
			for (int i = 1; i < clientSocketsVector.size() + 1; i++) {
				std::cout << "--Received: " << buf << std::endl;
				if (strncmp(buf,"LIST",4)==0)
				{
					std::cout << "--LIST----- " << std::endl;
					send(fds[i].fd,"",1,0);
				}
				if (strncmp(buf,"JOIN",4)==0)
				{
					std::cout << "--JOIN----- " << std::endl;
					// Envoi des messages au client
					//send_message(fds[i].fd, ":user JOIN :d\n");
					send_message(fds[i].fd, ":UserJID JOIN :ChannelName");
					send_message(fds[i].fd, ":Domain [RPL_TOPIC][1] NickName ChannelName :Topic");
					//send_message(fds[i].fd, ":Domain [RPL_NAMREPLY][1] NickName = ChannelName : NameList :Domain [RPL_ENDOFNAMES][1] NickName ChannelName :End of Names list");
				//for (int j = 1; j < clientSocketsVector.size() + 1; j++) {
				//	//printf("Received: %s", buf);
				//	send(fds[j].fd,buf, nbytes, 0);
				//}
				}
			}
		}
		*/