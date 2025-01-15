# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aduvilla <aduvilla@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/23 17:05:09 by eschussl          #+#    #+#              #
#    Updated: 2025/01/15 16:28:37 by aduvilla         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	ircserv

CXX 			=	c++ 
#CXX 			=	clang++ 

#CXXFLAGS		=	-fsanitize=address -std=c++98 -Wall -Wextra -Werror -g3
CXXFLAGS		=	-std=c++98 -Wall -Wextra -Werror -g3

COMMAND_PATH	= Commands/
SERVER_PATH		= Server/

SRCS		=	main.cpp\
				Client.cpp\
				Channel.cpp\
				utils.cpp\
				Parsing.cpp\
				serverExceptions.cpp\
				ChanMode.cpp\
				$(addprefix $(SERVER_PATH), Server.cpp initServer.cpp getServer.cpp signalServer.cpp authentificationServer.cpp runServer.cpp utilServer.cpp exitServer.cpp)\
				$(addprefix $(COMMAND_PATH), Nick.cpp Kick.cpp Join.cpp Ping.cpp PrivMsg.cpp Notice.cpp Part.cpp Topic.cpp Mode.cpp Quit.cpp Invite.cpp Who.cpp)
				
OBJS		=	$(addprefix .obj/,$(SRCS:.cpp=.o))

BOT_NAME	= 	ircbot

BOT_PATH	=	bot/

BOT_SRCS	=	$(addprefix $(BOT_PATH),Bot.cpp fileTransferBot.cpp main.cpp utils.cpp)

BOT_OBJS	=	$(addprefix .botObj/,$(BOT_SRCS:.cpp=.o))

all : $(NAME) $(BOT_NAME)

$(NAME):	$(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ -Iinc -I  inc/$(COMMAND_PATH)

.obj/%.o: src/%.cpp
	@mkdir -p .obj
	@mkdir -p .obj/$(SERVER_PATH)
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

clear :
	@rm *.log

-include $(OBJS:.o=.d)
-include $(BOT_OBJS:.o=.d)

.PHONY	: all clean fclean re bot clear
