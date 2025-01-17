/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 15:33:51 by eschussl          #+#    #+#             */
/*   Updated: 2025/01/14 17:08:30 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <arpa/inet.h>
#include "Server.hpp"
#include "Channel.hpp"
#include "utils.hpp"
#include "serverExceptions.hpp"

bool Server::m_signal = false;

/**
 * @brief Constructs a new Server object.
 * 
 * @param name The hostname of the server.
 * @param pass The password for the server.
 */
Server::Server(const std::string &name, const std::string &pass) : m_pass(pass), m_hostname(name)
{ 
	m_serverSocketFd = -1;
	m_nextChannelID = 0;
	std::string logName = name + ".log";
	m_logFd.open (logName.c_str(), std::fstream::in | std::fstream::out | std::fstream::app);
	if (!m_logFd.is_open())
		std::cerr << "Error : Couldn't open log file" << std::endl;
}

/**
 * @brief Creates a new channel without a password.
 * 
 * @param name The name of the channel.
 * @param client The client creating the channel.
 */
void	Server::createChannel(const std::string &name, Client &client)
{
	Channel* newChannel = new Channel(*this, name);
	newChannel->addClient(client, "");
	newChannel->addOP(client);
	m_vChannels.push_back(newChannel);
}

/**
 * @brief Creates a new channel with a password.
 * 
 * @param name The name of the channel.
 * @param client The client creating the channel.
 * @param passwd The password for the channel.
 */
void	Server::createChannel(const std::string &name, Client &client, const std::string &passwd)
{
	Channel*	newChannel = new Channel(*this, name, passwd);
	newChannel->addClient(client, passwd);
	newChannel->addOP(client);
	m_vChannels.push_back(newChannel);
}

/**
 * @brief Gets a client by their nickname.
 * 
 * @param nickname The nickname of the client.
 * @return A pointer to the Client if found, otherwise NULL.
 */
Client *Server::getClient(const std::string &nickname)
{
	for (size_t i = 0; i < m_vClients.size(); i++)
		if (strCompareNoCase(m_vClients[i]->getNickname(), nickname))
			return m_vClients[i];
	return NULL;
}
