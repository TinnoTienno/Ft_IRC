/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 14:05:05 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/14 00:17:11 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include <iomanip>
#include <sstream>
#include "serverExceptions.hpp"

/**
 * @brief  Gets the hostname of the server.
 *
 * @return The hostname as a string.
 */
const std::string Server::getHostname() const { return m_hostname; }

/**
 * @brief  Gets the number of users connected to the server.
 *
 * @return The number of users as a string.
 */
const std::string	Server::getUserNumber() const
{
	std::ostringstream	oss;
	oss << this->m_vClients.size();
	return oss.str();
}

/**
 * @brief  Gets the number of channels on the server.
 *
 * @return The number of channels as a string.
 */
const std::string	Server::getChannelNumber() const
{
	std::ostringstream	oss;
	oss << this->m_vChannels.size();
	return oss.str();
}

/**
 * @brief  Retrieves a client by their file descriptor.
 *
 * @param  clientFd : The file descriptor of the client.
 * @return A pointer to the client object, or NULL if not found.
 */
Client *Server::getClient(int clientFd) 
{
	for (size_t i = 0; i < m_vClients.size(); i++)
		if (clientFd == m_vClients[i]->getFD())
			return (m_vClients[i]);
	return NULL;
}

/**
 * @brief  Generates the next guest username.
 *
 * @return The next guest username as a string.
 */
const std::string Server::getNextGuest()
{
	static int i = 0;
	std::ostringstream	oss;
	oss << std::setfill ('0') << std::setw (3) <<  ++i;
	return ((std::string) "Guest" + oss.str());
}

/**
 * @brief  Gets the port number the server is running on.
 *
 * @return The port number as a string.
 */
const std::string	Server::getPort() const 
{
	std::ostringstream	oss;
	oss << this->m_port;
	return (std::string) oss.str();
}

/**
 * @brief  Retrieves a channel by its name.
 *
 * @param  channelName : The name of the channel.
 * @return A pointer to the channel object, or NULL if not found.
 */
Channel *Server::getChannel(const std::string &channelName)
{
	for (size_t i = 0; i < m_vChannels.size(); i++)
		if (m_vChannels[i]->getName() == channelName)
			return m_vChannels[i];
	return NULL;
}
