/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 16:56:14 by aduvilla          #+#    #+#             */
/*   Updated: 2024/12/13 16:14:53 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <sstream>
#include <iomanip>
#include "Parsing.hpp"

bool Server::isNickFormatted(const std::string &nickname) const
{
	if (nickname.find_first_of("@.:!") != nickname.npos || !nickname.find("#") || isdigit(nickname[0]))
		return 0;
	return 1;
}

int Server::findNick(const std::string &nickname) // carefull return index + 1 so not to get index == 0
{
	for (std::map<int, Client>::iterator iter = m_mClients.begin(); iter != m_mClients.end() ; iter++)
	{
		if (iter->second.getNick() == nickname)
			return iter->first;
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
	client.setUser(parse.getArguments()[1]);
	return 0;
}
