/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 15:33:51 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/21 08:25:44 by noda             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <arpa/inet.h>
#include "Server.hpp"
#include "Channel.hpp"
#include "Parsing.hpp"
#include "utils.hpp"
#include "serverExceptions.hpp"

bool Server::m_signal = false;

Server::Server(const std::string &name, const std::string &pass) : m_pass(pass), m_hostname(name)
{ 
	m_serverSocketFd = -1;
	m_nextChannelID = 0;
}

Server::~Server()
{
	for (std::map<int, Client>::iterator iter = m_mClients.begin(); iter != m_mClients.end(); iter++)
		ClearClient((*iter).second);
	CloseFds();
}

Channel &Server::createChannel(const std::string &name, Client &client)
{
	Channel newChannel(*this, name, client);
	m_vChannels.push_back(newChannel);
	std::cout << "New channel " << name << " was added " << client.getFD() << " is OP" << std::endl;
	Channel &tmp = m_vChannels[m_vChannels.size() - 1];
	std::cout << "tmp " << tmp.getName() << std::endl;
	client.addChannel(tmp);
	client.addOP(tmp);
	return tmp;
}

Channel &Server::createChannel(const std::string &name, Client &client, const std::string &passwd)
{
	Channel &channel = createChannel(name, client);
	channel.setPassword(passwd);
	return channel;
}

void 	Server::deleteChannel(Channel &channel)
{
	for (std::vector<Channel>::iterator iter = m_vChannels.begin(); iter != m_vChannels.end(); iter++)
	{
		if (iter->getName() == channel.getName())
		{
			std::cout << "Now deleting the " << channel.getName() << "channel" << std::endl;
			m_vChannels.erase(iter);
		}
	}
}

Client *Server::getClient(const std::string &nickname)
{
	for (size_t i = 0; i < m_mClients.size(); i++)
		if (strCompareNoCase(m_mClients[i].getNick(), nickname))
			return &m_mClients[i];
	return NULL;
}
