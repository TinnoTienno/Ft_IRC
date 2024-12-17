/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 16:56:14 by aduvilla          #+#    #+#             */
/*   Updated: 2024/12/16 18:40:11 by noda             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include <sstream>
#include <iomanip>
#include "Parsing.hpp"
#include <map>
#include <string>
#include "Channel.hpp"

int	Server::getNickFd(const int mapIndex) const
{
	if (this->m_mClients.find(mapIndex) == m_mClients.end())
		throw std::out_of_range("invalid mapIndex in m_mClients");
	return m_mClients.find(mapIndex)->second.getFD();
}

int Server::findChannel(const std::string &chanName)
{
	for (std::map<int, Channel>::iterator iter = m_mChannels.begin(); iter != m_mChannels.end() ; iter++)
	{
		if (iter->second.getName() == chanName)
			return iter->first;
	}
	return 0;
}

Channel	Server::getChan(const int mapIndex) const
{
	if (this->m_mChannels.find(mapIndex) == m_mChannels.end())
		throw std::out_of_range("invalid mapIndex in m_mClients");
	return m_mChannels.find(mapIndex)->second;
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
	client.setUser(parse.getArguments()[1]);
	client.setReal(parse.getArguments()[4]);
	return 0;
}

const std::string	Server::getPort() const 
{
	std::ostringstream	oss;
	oss << this->m_port;
	return (std::string) oss.str();
}

Channel *Server::findChannel(const std::string &channelName)
{
	for (size_t i = 0; i < m_vChannels.size(); i++)
		if (m_vChannels[i].getName() == channelName)
			return &m_vChannels[i];
	return NULL;
}
