/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exitServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 14:01:56 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/06 18:36:24 by noda             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include <unistd.h>
#include <iostream>
#include "Channel.hpp"

Server::~Server()
{
	CloseFds();
	m_vClients.clear();
	m_vFds.clear();
}

void	Server::CloseFds()
{
	for(size_t i = 0; i < m_vClients.size(); i++){ //-> close all the clients
		std::cout << RED << "Client <" << m_vClients[i].getFD() << "> Disconnected" << WHI << std::endl;
		close(m_vClients[i].getFD());
	}
	if (m_serverSocketFd != -1){ //-> close the server socket
		std::cout << RED << "Server <" << m_serverSocketFd << "> Disconnected" << WHI << std::endl;
		close(m_serverSocketFd);
	}
}
/*
void Server::ClearClient(Client &client)
{
	(void)client;
	m_mClients.erase(m_vFds[0].fd);
	close (m_vFds[0].fd);
	m_vFds.erase(m_vFds.begin() + 0);
}
*/
void Server::ClearClient(Client &client)
{
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
			m_vClients.erase(m_vClients.begin() + i);
			break;
		}
	}
}
