/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 17:43:19 by aduvilla          #+#    #+#             */
/*   Updated: 2024/12/13 15:56:36 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <ctime>
#include <netdb.h>
#include "Server.hpp"
#include <sstream>
#include <iomanip>
#include "Parsing.hpp"

void	sendMessage(const int fd, const std::string & source, const std::string & command, const std::string msg)
{
	std::string message = ":" + source + ".com " + command + " :" + msg + "\r\n";
	send(fd, message.c_str(), message.size(), 0);
	std::cout << message << std::endl;
}

const std::string	getTime()
{
	char				buffer[100];	
	std::time_t	now =std::time(0);
	std::tm	*timeStruct = std::gmtime(&now);
	std::strftime(buffer, sizeof(buffer), "%a %b %d %Y at %H:%M:%S GMT", timeStruct);
	return std::string(buffer);
}

bool Server::isNickFormatted(const std::string &nickname) const
{
	if (nickname.find_first_of("@.:!") != nickname.npos || !nickname.find("#") || isdigit(nickname[0]))
		return 0;
	return 1;
}

int Server::findNick(const std::string &nickname) const // carefull return index + 1 so not to get index == 0
{
	for (std::map<int, Client>::const_iterator iter = m_mClients.begin(); iter != m_mClients.end(); iter++)  
	{
		if ((*iter).second.getNick() == nickname)
			return (*iter).first;
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

bool Server::userErrorCode(Client &client, const Parsing &parse)
{
	(void) client;
	(void) parse;
	
	return 1;
}