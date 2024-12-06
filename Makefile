# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/23 17:05:09 by eschussl          #+#    #+#              #
#    Updated: 2024/12/06 17:23:33 by eschussl         ###   ########.fr        #
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
				$(addprefix $(COMMAND_PATH), ACommand.cpp Join.cpp)  
				
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
