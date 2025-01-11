/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 14:05:05 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/06 17:57:16 by noda             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include <iomanip>
#include <sstream>
#include "serverExceptions.hpp"

const std::string Server::getHostname() const { return m_hostname; }

const std::string	Server::getUserNumber() const
{
	std::ostringstream	oss;
	oss << this->m_vClients.size();
	return oss.str();
}

const std::string	Server::getChannelNumber() const
{
	std::ostringstream	oss;
	oss << this->m_vChannels.size();
	return oss.str();
}

Client *Server::getClient(int clientFd) 
{
	for (size_t i = 0; i < m_vClients.size(); i++)
	if (clientFd == m_vClients[i].getFD())
		return (&m_vClients[i]);
	return NULL;
}

const std::string Server::getNextGuest()
{
	static int i = 0;
	std::ostringstream	oss;
	oss << std::setfill ('0') << std::setw (3) <<  ++i;
	return ((std::string) "Guest" + oss.str());
}

const std::string	Server::getPort() const 
{
	std::ostringstream	oss;
	oss << this->m_port;
	return (std::string) oss.str();
}


Channel *Server::getChannel(const std::string &channelName)
{
	for (size_t i = 0; i < m_vChannels.size(); i++)
		if (m_vChannels[i].getName() == channelName)
			return &m_vChannels[i];
	return NULL;
}