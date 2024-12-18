# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aduvilla <aduvilla@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/23 17:05:09 by eschussl          #+#    #+#              #
<<<<<<< HEAD
#    Updated: 2024/12/17 16:34:03 by eschussl         ###   ########.fr        #
=======
#    Updated: 2024/12/17 14:19:23 by aduvilla         ###   ########.fr        #
>>>>>>> 2aef5294e1965bb4b2a8ba74b2a0886033821d3f
#                                                                              #
# **************************************************************************** #

NAME		=	ircserv

CC 			=	c++ 
CFLAGS		=	-std=c++98 -Wall -Wextra -Werror -g

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
<<<<<<< HEAD
				Exceptions.cpp\
=======
				signalServer.cpp\
				getServer.cpp\
				exitServer.cpp\
>>>>>>> 2aef5294e1965bb4b2a8ba74b2a0886033821d3f
				$(addprefix $(COMMAND_PATH), Nick.cpp Join.cpp UserHost.cpp Ping.cpp PrivMsg.cpp Notice.cpp)
				
OBJS		=	$(addprefix .obj/,$(SRCS:.cpp=.o))

all : $(NAME)

$(NAME):	$(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ -Iinc -I  inc/$(COMMAND_PATH)

.obj/%.o: src/%.cpp
	@mkdir -p .obj
	@mkdir -p .obj/$(COMMAND_PATH)
	$(CC) -MMD $(CFLAGS) -c $< -o $@ -Iinc -I inc/$(COMMAND_PATH)

clean :
	@rm -rf .obj

fclean : clean
	@rm -f $(NAME)

re : fclean $(NAME)

-include $(OBJS:.o=.d)
