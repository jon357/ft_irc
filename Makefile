# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jcheca <jcheca@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/15 12:13:50 by jcheca            #+#    #+#              #
#    Updated: 2024/06/12 14:51:11 by jcheca           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRC_DIR = src
INC_DIR = includes
OBJ_DIR = obj
LOG_DIR = logs

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

NAME = ircserv

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -o $@ $^ 
	mkdir -p $(LOG_DIR)
	touch $(LOG_DIR)/irc_server.log

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(NAME)

fclean: clean
	rm -rf $(LOG_DIR)

re: fclean all

.PHONY: all clean fclean re
