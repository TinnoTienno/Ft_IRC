# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/23 17:05:09 by eschussl          #+#    #+#              #
#    Updated: 2024/12/27 15:33:09 by aduvilla         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	ircserv

CXX 			=	c++ 
#CC 			=	clang++ 

CXXFLAGS		=	-std=c++98 -Wall -Wextra -Werror -g3
#CFLAGS		=	-Wall -Wextra -Werror -g3

COMMAND_PATH	= Commands/

SRCS		=	main.cpp\
				Client.cpp\
				Server.cpp\
				Channel.cpp\
				initServer.cpp\
				runServer.cpp\
				utilServer.cpp\
				utils.cpp\
				authentificationServer.cpp\
				Parsing.cpp\
				signalServer.cpp\
				getServer.cpp\
				exitServer.cpp\
				serverExceptions.cpp\
				$(addprefix $(COMMAND_PATH), Nick.cpp Join.cpp UserHost.cpp Ping.cpp PrivMsg.cpp Notice.cpp Quit.cpp)
				
OBJS		=	$(addprefix .obj/,$(SRCS:.cpp=.o))

BOT_NAME	= 	ircbot

BOT_PATH	=	bot/

BOT_SRCS	=  $(addprefix $(BOT_PATH),Bot.cpp main.cpp)

BOT_OBJS	= $(addprefix .botObj/,$(BOT_SRCS:.cpp=.o))

all : $(NAME) $(BOT_NAME)

$(NAME):	$(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ -Iinc -I  inc/$(COMMAND_PATH)

.obj/%.o: src/%.cpp
	@mkdir -p .obj
	@mkdir -p .obj/$(COMMAND_PATH)
	$(CXX) -MMD $(CXXFLAGS) -c $< -o $@ -Iinc -I inc/$(COMMAND_PATH)

bot			:	$(BOT_NAME)

$(BOT_NAME)	:	$(BOT_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@  -Ibot

.botObj/%.o: %.cpp
	@mkdir -p .botObj/$(BOT_PATH)
	$(CXX) -MMD $(CXXFLAGS) -c $< -o $@ -Ibot

clean :
	@rm -rf .obj
	@rm -rf .botObj

fclean : clean
	@rm -f $(NAME)
	@rm -f $(BOT_NAME)

re : fclean $(NAME) $(BOT_NAME)

-include $(OBJS:.o=.d)
-include $(BOT_OBJS:.o=.d)
