/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 15:33:51 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/12 00:42:54 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include "Server.hpp"
#include "Join.hpp"
#include "Nick.hpp"
#include "Channel.hpp"
#include "UserHost.hpp"

bool Server::m_signal = false;

Server::Server(const std::string &name, const std::string &pass) : m_pass(pass), m_hostname(name)
{ 
	m_serverSocketFd = -1;
}

Server::~Server()
{
	for (size_t i = 0; i < m_vFds.size(); i++)
		ClearClients(m_vFds[i].fd);
	CloseFds();
}



void Server::SignalHandler(int signum)
{
	(void)signum;
	std::cout << std::endl << "Signal Received!" << std::endl;
	Server::m_signal = true; //-> set the static boolean to true to stop the server
}

void	Server::CloseFds(){
	for(size_t i = 0; i < m_vClients.size(); i++){ //-> close all the clients
		std::cout << RED << "Client <" << m_vClients[i].getFD() << "> Disconnected" << WHI << std::endl;
		close(m_vClients[i].getFD());
	}
	if (m_serverSocketFd != -1){ //-> close the server socket
		std::cout << RED << "Server <" << m_serverSocketFd << "> Disconnected" << WHI << std::endl;
		close(m_serverSocketFd);
	}
}

void Server::ClearClients(int fd)
{
	for (size_t i = 0; i < m_vFds.size(); i++)
	{
		if (m_vFds[i].fd == fd)
		{
			m_vFds.erase(m_vFds.begin() + i);
			break;	
		}
	}
	for (size_t i = 0; i < m_vClients.size(); i++)
	{
		if (m_vClients[i].getFD() == fd)
		{
			m_vClients.erase(m_vClients.begin() + i);
			break;
		}
	}
	close (fd);
}

void Server::parseCommand(const std::string buffer, Client &client)
{
	std::string		Commands[] = {"JOIN", "NICK", "userhost"};	
	void (*fCommands[])(Server *, const std::string, Client &) = { &Join::execute, &Nick::execute, &UserHost::execute };
	size_t size = sizeof(Commands) / sizeof(Commands[0]);
	std::cout << "|" << buffer << "|" << std::endl;
	for (size_t i = 0; i < size; i++)
	{
		if (buffer.find(Commands[i]) == 0)
		{
			std::string line = buffer.substr(Commands[i].size() + 1, buffer.npos);
			fCommands[i](this, line, client);
		}
	}
}

const std::string Server::getHostname() const { return m_hostname; }

//:irc.njip.com 001 0002 003
//:nick!user@host NICK TOPIC PRIVMSG

bool Server::findNick(const std::string &nickname)
{
	for (size_t i = 0; i < m_vClients.size(); i++)
	{
		if (nickname == m_vClients[i].getNick())
			return 1;
	}
	return 0;
}

const std::string	Server::getUserNumber() const
{
	return itoa(this->m_vClients.size());
}

const std::string	Server::getChannelNumber() const
{
	return itoa(this->m_vChannels.size());
}
