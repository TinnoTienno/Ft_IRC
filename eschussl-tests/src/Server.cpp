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
#include <fcntl.h>

Server::Server() { m_serverSocketFd = -1; }

void Server::ServerInit(const std::string &port)
{
	if (port.find_first_not_of("0123456789") != std::string::npos)
		throw std::invalid_argument("error: invalid argument: port: " + port);
	this->m_port = atoi(port.c_str());
	SerSocket();
	std::cout << "Server <" << m_serverSocketFd << "> Connected" << std::endl;
	std::cout << "Waiting for a connection..." << std::endl;

}
void Server::SerSocket()
{
	struct sockaddr_in socketAdd;
	struct pollfd Poll;

	socketAdd.sin_family = AF_INET;
	socketAdd.sin_port = htons(this->m_port);
	socketAdd.sin_addr.s_addr = INADDR_ANY;
	
	m_serverSocketFd = socket(AF_INET, SOCK_STREAM, 0);
	if(m_serverSocketFd == -1)
		throw std::runtime_error("socket");
	int i = 1;

	if (setsockopt(m_serverSocketFd, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(i)) < 0)
		throw std::runtime_error("failed to set socket option: SO_REUSEADDR");
	if (fcntl(m_serverSocketFd, F_SETFL, O_NONBLOCK) < 0)
		throw std::runtime_error("failed to set socket: O_NONBLOCK");
	if (bind(m_serverSocketFd, (struct sockaddr *)&socketAdd, sizeof(socketAdd)) < 0)
		throw std::runtime_error("failed to bind socket to address");
	if (listen(m_serverSocketFd, SOMAXCONN) < 0)
		throw std::runtime_error("failed to listen");
	Poll.fd = m_serverSocketFd;
	Poll.events = POLLIN;
	Poll.revents = 0;
	m_fds.push_back(Poll);
	std::cout << "Server listening on port " << m_port << std::endl;
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