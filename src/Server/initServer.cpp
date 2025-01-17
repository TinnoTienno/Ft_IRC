/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduvilla <aduvilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 16:00:52 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/13 14:37:09 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <exception>
#include <stddef.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/types.h>
#include <stddef.h>
#include "serverExceptions.hpp"
#include "utils.hpp"

/**
 * @brief  Initializes the server with the specified port.
 *
 * This function sets up the server by converting the port string to an integer,
 * creating and binding a socket, and starting to listen for incoming connections.
 * It also manages incoming connections and data using the poll mechanism.
 *
 * @param  port : Port number as a string to initialize the server.
 */
void Server::ServerInit(const std::string &port)
{
	try
	{
		this->m_port = strtous(port);
		SerSocket();
		sendLog("Server <" + itoa(m_serverSocketFd) + "> Connected");
		sendLog("Waiting for a connection...");
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
						ReceiveNewData(*getClient(m_vFds[i].fd));
				}
			}
		}
	}
	catch (std::exception & e)
	{
		sendLog(e.what());
	}
}

/**
 * @brief  Configures the server socket and prepares it for connections.
 *
 * This function creates a socket, sets it to non-blocking mode,
 * binds it to the specified port, and starts listening for connections.
 * It also initializes the poll structure for the socket.
 *
 * @throws std::runtime_error if socket creation, configuration, binding, or listening fails.
 */
void Server::SerSocket()
{
	struct sockaddr_in socketAdd = {};
	struct pollfd Poll;

	socketAdd.sin_family = AF_INET;
	socketAdd.sin_port = htons(this->m_port);
	socketAdd.sin_addr.s_addr = INADDR_ANY;
	
	m_serverSocketFd = socket(AF_INET, SOCK_STREAM, 0);
	if(m_serverSocketFd == -1)
		throw std::runtime_error("Error: Socket creation failed");
	int i = 1;

	if (setsockopt(m_serverSocketFd, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(i)) < 0)
		throw std::runtime_error("Error: Failed to set socket option: SO_REUSEADDR");
	if (fcntl(m_serverSocketFd, F_SETFL, O_NONBLOCK) < 0) // as asked in the subject
		throw std::runtime_error("Error: Failed to set socket: O_NONBLOCK");
	if (bind(m_serverSocketFd, (struct sockaddr *)&socketAdd, sizeof(socketAdd)) < 0)
		throw std::runtime_error("Error: Failed to bind socket to address");
	if (listen(m_serverSocketFd, SOMAXCONN) < 0)
		throw std::runtime_error("Error: Failed to listen");
	Poll.fd = m_serverSocketFd;
	Poll.events = POLLIN;
	Poll.revents = 0;
	m_vFds.push_back(Poll);
	sendLog("Server listening on port " + itoa(m_port));
}
