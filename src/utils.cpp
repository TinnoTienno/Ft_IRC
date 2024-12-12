/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 17:43:19 by aduvilla          #+#    #+#             */
/*   Updated: 2024/12/12 15:38:01 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <netdb.h>
#include "Server.hpp"
#include <sstream>
#include <iomanip>

void	sendMessage(const int fd, const std::string & source, const std::string & command, const std::string msg)
{
	std::string message = ":" + source + ".com " + command + " :" + msg + "\r\n";
	send(fd, message.c_str(), message.size(), 0);
	std::cout << message << std::endl;
}

bool Server::isNickFormatted(const std::string &nickname) const
{
	if (nickname.find_first_of("@.:!") != nickname.npos || !nickname.find("#") || isdigit(nickname[0]))
		return 0;
	return 1;
}

int Server::findNick(const std::string &nickname) const // carefull return index + 1 so not to get index == 0
{
	for (size_t i = 0; i < m_vClients.size(); i++)
	{
		if (m_vClients[i].getNick() == nickname)
		{
			return i + 1;
		}
	}
	return 0;
}

const std::string Server::getNextGuest()
{
	static int i = 0;
	std::ostringstream	oss;
	oss << std::setfill ('0') << std::setw (3) <<  ++i;
	
	return ((std::string) "Guest" + oss.str());
}

bool Server::userErrorCode(Client &client, const std::string &buffer)
{
	(void) client;
	(void) buffer;
	struct parse
	{
		std::string cmd;
	}	;
	return 1;
}