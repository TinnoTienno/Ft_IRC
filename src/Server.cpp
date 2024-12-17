/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 15:33:51 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/17 11:55:31 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <arpa/inet.h>
#include "Server.hpp"
#include "Join.hpp"
#include "Nick.hpp"
#include "Channel.hpp"
#include "UserHost.hpp"
#include "Ping.hpp"
#include "PrivMsg.hpp"
#include "Notice.hpp"
#include "Parsing.hpp"

bool Server::m_signal = false;

Server::Server(const std::string &name, const std::string &pass) : m_pass(pass), m_hostname(name)
{ 
	m_serverSocketFd = -1;
}

Server::~Server()
{
	for (std::map<int, Client>::iterator iter = m_mClients.begin(); iter != m_mClients.end(); iter++)
		ClearClient((*iter).second);
	CloseFds();
}

void Server::SignalHandler(int signum)
{
	(void)signum;
	std::cout << std::endl << "Signal Received!" << std::endl;
	Server::m_signal = true; //-> set the static boolean to true to stop the server
}

void	Server::CloseFds()
{
	for(size_t i = 0; i < m_mClients.size(); i++){ //-> close all the clients
		std::cout << RED << "Client <" << m_mClients[i].getFD() << "> Disconnected" << WHI << std::endl;
		close(m_mClients[i].getFD());
	}
	if (m_serverSocketFd != -1){ //-> close the server socket
		std::cout << RED << "Server <" << m_serverSocketFd << "> Disconnected" << WHI << std::endl;
		close(m_serverSocketFd);
	}
}

void Server::ClearClient(Client &client)
{
	for (size_t i = 0; i < m_vFds.size(); i++)
	{
		if (m_vFds[i].fd == client.getFD())
		{
			m_mClients.erase(m_vFds[i].fd);
			close (m_vFds[i].fd);
			m_vFds.erase(m_vFds.begin() + i);
			break;	
		}
	}
}

void Server::parseCommand(const std::string line, Client &client)
{
	std::string		Commands[] = {"JOIN", "NICK", "userhost", "PING", "PRIVMSG", "NOTICE"};
	void (*fCommands[])(Server *, const Parsing &, Client &) = { &Join::execute,
		&Nick::execute,
		&UserHost::execute,
		&Ping::execute,
		&PrivMsg::execute,
		&Notice::execute};
	size_t size = sizeof(Commands) / sizeof(Commands[0]);
	Parsing parse(line);
	for (size_t i = 0; i < size; i++)
	{
		if (parse.getCommand() == Commands[i])
			fCommands[i](this, parse, client);
	}
}

const std::string Server::getHostname() const { return m_hostname; }

//:irc.njip.com 001 0002 003
//:nick!user@host NICK TOPIC PRIVMSG
const std::string	Server::getUserNumber() const
{
	std::ostringstream	oss;
	oss << this->m_mClients.size();
	return oss.str();
}

const std::string	Server::getChannelNumber() const
{
	std::ostringstream	oss;
	oss << this->m_mChannels.size();
	return oss.str();
}

Client &Server::getClient(int clientKey)
{
	return (m_mClients.find(clientKey)->second);
}
