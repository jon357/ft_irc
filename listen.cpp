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
		int ret = poll(fds, clientSocketsVector.size() + 1, -1);
		memset(buf, '\0', sizeof(buf));
		int nbytes = recv(fds[ret].fd, buf, sizeof(buf), 0);
		(void)nbytes;
		std::cout << "____ " << ret << " " << sizeof(buf) << " " << buf << std::endl;

		if (strncmp(buf, "NICK", 4) == 0)
		{
			int i = 0;
			while(buf[i])
			{
				if (buf[i] == '\n')
					std::cout << " = " << std::endl;
				else
					std::cout << buf[i] << "-";
				i++;
			}
		}



		if (ret < 0)
		{
			perror("error poll");
			exit(EXIT_FAILURE);
		}

		if (fds[0].revents & POLLIN)
		{
			int new_fd = accept(sockfd, NULL, NULL);
			if (new_fd < 0)
			{
				perror("error accept");
				continue;
			}
			std::cout << "New connection accepted." << std::endl;
			fds[clientSocketsVector.size() + 1].fd = new_fd;
			fds[clientSocketsVector.size() + 1].events = POLLIN;
			clientSocketsVector.push_back(new_fd);
			send(new_fd, "Welcome to the IRC server!\n", 27, 0);
			//on--;
		}
		if (strncmp(buf,"QUIT",4)==0)
			clientSocketsVector.erase(clientSocketsVector.begin() + (ret - 1));
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