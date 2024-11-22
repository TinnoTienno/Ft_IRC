/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 15:33:51 by eschussl          #+#    #+#             */
/*   Updated: 2024/11/22 17:50:16 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <stdexcept>
#include <stdlib.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

Server::Server() { m_serverSocketFd = -1; }

void Server::ServerInit(const std::string &port)
{
	if (port.find_first_not_of("0123456789") != std::string::npos)
		throw std::invalid_argument("error: invalid argument: " + port);
	this->m_port = atoi(port.c_str());
	SerSocket();
	std::cout << "Server <" << m_serverSocketFd << "> Connected" << std::endl;
	std::cout << "Waiting for a connection..." << std::endl;
}
void Server::SerSocket()
{
	struct sockaddr_in socket;
	struct pollfd Poll;

	socket.sin_family = AF_INET;
	socket.sin_port = htons(this->m_port);
	socket.sin_addr.s_addr = INADDR_ANY;
	
	m_serverSocketFd = socket(AF_INET, SOCK_STREAM, 0);
	if(m_serverSocketFd)
}
void Server::AcceptNewCLient()
{
	
}
void Server::ReceiveNewData(const int&)
{
	
}

// static void Server::SignalHandler(int)
// {
	
// }

void Server::CloseFds()
{
	
}
void Server::ClearClients(int)
{
	
}