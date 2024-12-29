/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 16:00:52 by aduvilla          #+#    #+#             */
/*   Updated: 2024/12/29 08:55:31 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <exception>
#include <iostream>
#include <stddef.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/types.h>
#include <stddef.h>
#include "serverExceptions.hpp"

void Server::ServerInit(const std::string &port)
{
	if (port.find_first_not_of("0123456789") != std::string::npos)
		throw std::invalid_argument("error: invalid argument: port: " + port);
	this->m_port = atoi(port.c_str());
	try
	{
		SerSocket();
		std::cout << "Server <" << m_serverSocketFd << "> Connected" << std::endl;
		std::cout << "Waiting for a connection..." << std::endl;
		while (m_signal == false)
		{
			if ((poll(&m_vFds[0], m_vFds.size(), -1) == -1) && m_signal == false)
				throw(std::runtime_error("Poll failed"));
			for (size_t i = 0; i < m_vFds.size(); i++)
			{
				if (m_vFds[i].revents & POLLIN)
				{
					if (m_vFds[i].fd == m_serverSocketFd)
						AcceptNewClient();
					else
						ReceiveNewData(m_mClients[m_vFds[i].fd]);
				}
			}
		}
	}
	catch (std::exception & e)
	{
		std::cerr << e.what() << std::endl;
	}
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
	if (fcntl(m_serverSocketFd, F_SETFL, O_NONBLOCK) < 0) // as asked in the subject
		throw std::runtime_error("failed to set socket: O_NONBLOCK");
	if (bind(m_serverSocketFd, (struct sockaddr *)&socketAdd, sizeof(socketAdd)) < 0)
		throw std::runtime_error("failed to bind socket to address");
	if (listen(m_serverSocketFd, SOMAXCONN) < 0)
		throw std::runtime_error("failed to listen");
	Poll.fd = m_serverSocketFd;
	Poll.events = POLLIN;
	Poll.revents = 0;
	m_vFds.push_back(Poll);
	std::cout << "Server listening on port " << m_port << std::endl;
}
