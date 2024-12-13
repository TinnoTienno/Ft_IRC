/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 16:09:09 by aduvilla          #+#    #+#             */
/*   Updated: 2024/12/13 15:28:50 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>
#include <fcntl.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <map>

void Server::AcceptNewClient()
{
	Client client;
	struct sockaddr_in clientAdd;
	struct pollfd newPoll;
	socklen_t len = sizeof(clientAdd);

	int incoFd = accept(m_serverSocketFd, (sockaddr *)&(clientAdd), &len);
	if (incoFd == -1)
	{
		std::cout << "accept() has failed" << std::endl;
		return;
	}
	if (fcntl(incoFd, F_SETFL, O_NONBLOCK) == -1)
	{
		std::cout << "fcntl() has failed" << std::endl;
		return;
	}
	newPoll.fd = incoFd;
	newPoll.events = POLLIN;
	newPoll.revents = 0;
	
	client.setFD(incoFd);
	client.setIPadd(inet_ntoa((clientAdd.sin_addr)));
	// m_mClients.insert({incoFd, client});
	m_mClients[incoFd] = client;
	m_vFds.push_back(newPoll);
	std::cout << GRE << "Client <" << incoFd << "> Connected" << WHI << std::endl;
}

void Server::ReceiveNewData(Client &client)
{
	char buff[513]; //-> buffer for the received data
	memset(buff, 0, sizeof(buff)); //-> clear the buffer
	ssize_t bytes = recv(client.getFD(), buff, sizeof(buff) - 1, 0); //-> receive the data

	if (bytes <= 0){ //-> check if the client disconnected
		std::cout << RED << "Client <" << client.getFD() << "> Disconnected" << WHI << std::endl;
		ClearClient(client); //-> clear the client
	}
	else //-> print the received data
	{
		buff[bytes] = '\0';
		std::string line = parseBuffer(client, buff);
		if (checkAuth(client, line))
		{
			std::cout << "line " << line << std::endl;
			for (size_t i = 0; i < line.size(); i++)
				std::cout << (int) line[i] << " ";
			std::cout << std::endl;
			size_t j = 0;
			for (size_t k = 0; k < line.size(); k = j + 1)
			{
				j = line.find("\n", k + 1);
				parseCommand(line.substr(k, j - k), client);
			}
		}
	}
}

std::string Server::parseBuffer(Client &client, std::string buffer)
{
	if (buffer.find("\r\n") == buffer.npos)
	{
		client.addPacket(buffer);
		return "";
	}
	std::string line = client.getPacket() + buffer; //careful this one might bring bugs if /r/n is not the end of the string
	return line;
} 
