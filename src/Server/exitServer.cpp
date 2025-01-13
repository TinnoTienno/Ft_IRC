/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exitServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduvilla <aduvilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 14:01:56 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/13 19:44:01 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include <unistd.h>
#include "Channel.hpp"
#include "utils.hpp"

Server::~Server()
{
	CloseFds();
	m_vClients.clear();
	m_vFds.clear();
	sendLog("closing");
	sendLog("\n============================================\n");
	m_logFd.close();
}

void	Server::CloseFds()
{
	for(size_t i = 0; i < m_vClients.size(); i++){ //-> close all the clients
		sendLog("Client <" + itoa(m_vClients[i].getFD()) + "> Disconnected");
		close(m_vClients[i].getFD());
	}
	if (m_serverSocketFd != -1){ //-> close the server socket
		sendLog("Server <" + itoa(m_serverSocketFd) + "> Disconnected");
		close(m_serverSocketFd);
	}
}

#include <iostream>
void Server::ClearClient(Client &client)
{
	client.cleanChannels();
	close (client.getFD());
	for (size_t i = 0; i < m_vFds.size(); i++)
	{
		if (m_vFds[i].fd == client.getFD())
		{
			m_vFds.erase(m_vFds.begin() + i);
			break;
		}
	}
	for (size_t i = 0; i < m_vClients.size(); i++)
	{
		if (&m_vClients[i] == &client)
		{
			sendLog("deleting client : " + m_vClients[i].getNickname());
			m_vClients.erase(m_vClients.begin() + i);
			break;
		}
	}
	std::cout << "supprimed " << std::endl;
	sendLog("Bug checking : vclient size : " + itoa(m_vClients.size()));
}
