/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 16:09:09 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/08 14:44:24 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>
#include <fcntl.h>
#include <arpa/inet.h>
#include <string>
#include <unistd.h>
#include <string.h>
#include "UserHost.hpp"
#include "Ping.hpp"
#include "Topic.hpp"
#include "PrivMsg.hpp"
#include "Notice.hpp"
#include "Join.hpp"
#include "Nick.hpp"
#include "Part.hpp"
#include "Kick.hpp"
#include "Quit.hpp"
#include <map>
#include "serverExceptions.hpp"
#include "Mode.hpp"
#include "utils.hpp"

void Server::AcceptNewClient()
{
	Client client;
	struct sockaddr_in clientAdd;
	struct pollfd newPoll;
	socklen_t len = sizeof(clientAdd);

	int incoFd = accept(m_serverSocketFd, reinterpret_cast<struct sockaddr*>(&clientAdd), &len);
	if (incoFd == -1)
	{
		sendLog("accept() has failed");
		return;
	}
	if (fcntl(incoFd, F_SETFL, O_NONBLOCK) == -1)
	{
		sendLog("fcntl() has failed");
		return;
	}
	newPoll.fd = incoFd;
	newPoll.events = POLLIN;
	newPoll.revents = 0;
	
	client.setFD(incoFd);
	client.setIPadd(inet_ntoa((clientAdd.sin_addr)));
	client.setHost(reinterpret_cast<struct sockaddr*>(&clientAdd), *this);
	// m_mClients.insert({incoFd, client});
	m_vClients.push_back(client);
	m_vFds.push_back(newPoll);
	sendLog("Client <" + itoa(incoFd) + "> Connected");
}

void Server::ReceiveNewData(Client &client)
{
	char buff[513]; //-> buffer for the received data
	memset(buff, 0, sizeof(buff)); //-> clear the buffer
	ssize_t bytes = recv(client.getFD(), buff, sizeof(buff) - 1, 0); //-> receive the data

	if (bytes <= 0){ //-> check if the client disconnected
		sendLog(static_cast<std::string>("Client <" + itoa(client.getFD()) + "> Disconnected"));
		ClearClient(client); //-> clear the client
	}
	else //-> print the received data
	{
		buff[bytes] = '\0';
		std::string line = parseBuffer(client, buff);
		if (checkAuth(client, line))
		{
			size_t j = 0;
			for (size_t k = 0; k < line.size(); k = j + 1)
			{
				j = line.find("\n", k + 1);
				parseCommand(line.substr(k, j - k - 1), client);
			}
		}
	}
}

void Server::parseCommand(const std::string line, Client &client)
{
	std::string		Commands[] = {"JOIN", 
	"NICK", 
	"userhost", 
	"PING", 
	"PRIVMSG", 
	"NOTICE", 
	"PART", 
	"TOPIC", 
	"KICK", 
	"MODE",
	"QUIT"};
	void (*fCommands[])(Server &, const Parsing &, Client &) = { &Join::execute,
		&Nick::execute,
		&UserHost::execute,
		&Ping::execute,
		&PrivMsg::execute,
		&Notice::execute,
		&Part::execute,
		&Topic::execute,
		&Kick::execute,
		&Mode::execute,
		&Quit::execute};
	size_t size = sizeof(Commands) / sizeof(Commands[0]);
	sendLog(itoa(client.getFD()) + " >> " + line);
	Parsing parse(line);
	for (size_t i = 0; i < size; i++)
	{
		if (parse.getCommand() == Commands[i])
			fCommands[i](*this, parse, client);
	}
}

std::string Server::parseBuffer(Client &client, std::string buffer)
{
	if (buffer.find("\n") == buffer.npos)
	{
		client.addPacket(buffer);
		return "";
	}
	size_t	pos = 0;
	while ((pos = buffer.find('\n', pos)) != std::string::npos)
	{
		if (pos == 0 || buffer[pos - 1] != '\r')
		{
			buffer.insert(pos, 1, '\r');
			pos++;
		}
		pos++;
	}
	std::string result = client.getPacket() + buffer; //careful this one might bring bugs if /r/n is not the end of the string
	size_t	lastN = result.rfind('\n');
	client.addPacket(result.substr(lastN + 1));
	return result.substr(0, lastN + 1);
}
/*
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
*/
