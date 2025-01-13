/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 15:33:51 by eschussl          #+#    #+#             */
/*   Updated: 2025/01/14 00:15:08 by aduvilla         ###   ########.fr       */
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

Server::Server(const std::string &name, const std::string &pass) : m_pass(pass), m_hostname(name)
{ 
	m_serverSocketFd = -1;
	m_nextChannelID = 0;
	std::string logName = name + ".log";
	m_logFd.open (logName.c_str(), std::fstream::in | std::fstream::out | std::fstream::app);
	if (!m_logFd.is_open())
		std::cerr << "Error : Couldn't open log file" << std::endl;
	sendLog("Server created");
}

void	Server::createChannel(const std::string &name, Client &client)
{
	Channel* newChannel = new Channel(*this, name);
	newChannel->addClient(client, "");
	newChannel->addOP(client);
	m_vChannels.push_back(newChannel);
	sendLog("New channel " + name + " was added " + itoa(client.getFD()) + " is OP");
}

void	Server::createChannel(const std::string &name, Client &client, const std::string &passwd)
{
	Channel*	newChannel = new Channel(*this, name);
	newChannel->addClient(client, passwd);
	newChannel->addOP(client);
	m_vChannels.push_back(newChannel);
	sendLog("New channel " + name + " was added " + itoa(client.getFD()) + " is OP");
}

/*
Channel &Server::createChannel(const std::string &name, Client &client)
{
	Channel newChannel(*this, name);
	m_vChannels.push_back(newChannel);
	sendLog("New channel " + name + " was added " + itoa(client.getFD()) + " is OP");
	Channel &tmp = m_vChannels[m_vChannels.size() - 1];
	sendLog("tmp " + tmp.getName());
	m_vChannels[m_vChannels.size() - 1].addClient(client, "");
	m_vChannels[m_vChannels.size() - 1].addOP(client);
	return tmp;
}

Channel &Server::createChannel(const std::string &name, Client &client, const std::string &passwd)
{
	Channel &channel = createChannel(name, client);
	channel.getMode()->setPassword(passwd);
	return channel;
}
*/
void 	Server::deleteChannel(Channel &channel)
{
	for (size_t i = 0; i < m_vChannels.size(); i++) {
		if (m_vChannels[i] == &channel)
		{
			sendLog("Now deleting the " + m_vChannels[i]->getName() + " channel");
			m_vChannels.erase(m_vChannels.begin() + i);
		}
	}
}

Client *Server::getClient(const std::string &nickname)
{
	for (size_t i = 0; i < m_vClients.size(); i++)
		if (strCompareNoCase(m_vClients[i]->getNickname(), nickname))
			return m_vClients[i];
	return NULL;
}
