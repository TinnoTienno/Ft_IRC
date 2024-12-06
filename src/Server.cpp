/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 15:33:51 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/06 18:10:25 by aduvilla         ###   ########.fr       */
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
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define RED "\e[1;31m" //-> for red color
#define WHI "\e[0;37m" //-> for white color
#define GRE "\e[1;32m" //-> for green color
#define YEL "\e[1;33m" //-> for yellow color

Server::Server(const std::string &pass) : m_pass(pass)
{ m_serverSocketFd = -1; }

bool Server::m_signal = false;

Server::~Server()
{
	for (size_t i = 0; i < m_fds.size(); i++)
		ClearClients(m_fds[i].fd);
	CloseFds();
}

void Server::ServerInit(const std::string &port)
{
	if (port.find_first_not_of("0123456789") != std::string::npos)
		throw std::invalid_argument("error: invalid argument: port: " + port);
	this->m_port = atoi(port.c_str());
	SerSocket();
	std::cout << "Server <" << m_serverSocketFd << "> Connected" << std::endl;
	std::cout << "Waiting for a connection..." << std::endl;
	while (m_signal == false)
	{
		if ((poll(&m_fds[0], m_fds.size(), -1) == -1) && m_signal == false)
			throw(std::runtime_error("Poll failed"));
		for (size_t i = 0; i < m_fds.size(); i++)
		{
			if (m_fds[i].revents & POLLIN)
			{
				if (m_fds[i].fd == m_serverSocketFd)
				{
					AcceptNewClient();
				}
				else
				{
					ReceiveNewData(m_fds[i].fd);
				}
			}
		}
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
	m_fds.push_back(Poll);
	std::cout << "Server listening on port " << m_port << std::endl;
}

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
	m_clients.push_back(client);
	m_fds.push_back(newPoll);
	std::cout << GRE << "Client <" << incoFd << "> Connected" << WHI << std::endl;
}
bool Server::checkNick(const size_t &clientid, const std::string &buffer)
{
	size_t nick = buffer.find("NICK");
	nick = buffer.find(' ', nick) + 1;
	size_t nick2 = buffer.find(13, nick);
	std::string  nickname = buffer.substr(nick, nick2 - nick);
	std::cout << "nick |" << nickname << "|" << std::endl;
	m_clients[clientid].setNick(nickname);
	for (size_t i = 0; i < m_clients.size(); i++)
	{
		if (m_clients[i].getNick() == nickname && i != clientid)
		{
			std::cout << RED << "Client <" << m_clients[clientid].getFD() << "> nickname already used" << WHI << std::endl;
			m_clients[clientid].kill("nickname already taken");
			ClearClients(m_clients[clientid].getFD());
			return 0;
		}
	}
	return 1;
}
bool Server::checkAuth(int fd, const std::string &buffer)
{
	size_t i = 0;
	for (; i < m_clients.size(); i++)
	{
		if (m_clients[i].getFD() == fd)
			break ;
	}
	if (m_clients[i].getAuth() == 1)
		return 1;
	if (buffer.find("USER") == buffer.npos)
	    return 0;
	if (!checkNick(i, buffer))
		return 0;
	size_t pass = buffer.find("PASS");
	if (pass == buffer.npos)
	{
	    std::cout << RED << "Client <" << fd << "> has not set a password" << WHI << std::endl;
		m_clients[i].sendMsg("please send a password");
		return 0;
	}
	pass = buffer.find(' ', pass) + 1;
	size_t pass2 = buffer.find(13, pass);
	// std::cout << "pass : " << pass << " pass2 : " << pass2 << std::endl;
	std::string passwd = buffer.substr(pass, pass2 - pass);
	std::cout << "pass : |" << passwd << "|" << std::endl;
	// std::cout << "m_pass : |" << m_pass << "|" << std::endl;
	if (!passwd.compare(m_pass))
	{
		m_clients[i].setAuth(1);
		std::cout << GRE << "Client <" << fd << "> is now authentified" << WHI << std::endl;
		m_clients[i].sendMsg("you're now authentified");
		return 0;
	}
	std::cout << RED << "Client <" << fd << "> has a wrong password" << WHI << std::endl;
	m_clients[i].kill("password is wrong");
	ClearClients(fd);
	return 0;
}

void Server::ReceiveNewData(const int fd)
{
	char buff[513]; //-> buffer for the received data
	memset(buff, 0, sizeof(buff)); //-> clear the buffer

	ssize_t bytes = recv(fd, buff, sizeof(buff) - 1, 0); //-> receive the data

	if (bytes <= 0){ //-> check if the client disconnected
		std::cout << RED << "Client <" << fd << "> Disconnected" << WHI << std::endl;
		ClearClients(fd); //-> clear the client
		close(fd); //-> close the client socket
	}

	else{ //-> print the received data
		buff[bytes] = '\0';
		if (checkAuth(fd, buff))
			std::cout << YEL << "Client <" << fd << "> Data: " << WHI << buff;
		
			// std::cout << RED << "Client <" << fd << "> is not authorized" << WHI << std::endl;
	}
}

void Server::SignalHandler(int signum)
{
	(void)signum;
	std::cout << std::endl << "Signal Received!" << std::endl;
	Server::m_signal = true; //-> set the static boolean to true to stop the server
}

void	Server::CloseFds(){
	for(size_t i = 0; i < m_clients.size(); i++){ //-> close all the clients
		std::cout << RED << "Client <" << m_clients[i].getFD() << "> Disconnected" << WHI << std::endl;
		close(m_clients[i].getFD());
	}
	if (m_serverSocketFd != -1){ //-> close the server socket
		std::cout << RED << "Server <" << m_serverSocketFd << "> Disconnected" << WHI << std::endl;
		close(m_serverSocketFd);
	}
}

void Server::ClearClients(int fd)
{
	for (size_t i = 0; i < m_fds.size(); i++)
	{
		if (m_fds[i].fd == fd)
		{
			m_fds.erase(m_fds.begin() + i);
			break;	
		}
	}
	for (size_t i = 0; i < m_clients.size(); i++)
	{
		if (m_clients[i].getFD() == fd)
		{
			m_clients.erase(m_clients.begin() + i);
			break;
		}
	}
	close (fd);
}

void registerCommand(const std::string& command, commandHandler handler)
{
	commandHandlers[command] = handler;
}
    
void handleCommand(const std::string& command, const std::string& params)
{
	if (commandHandlers.find(command) != commandHandlers.end()) // Appel de la méthode via un pointeur sur fonction membre
		(this->*commandHandlers[command])(params);
	else
		std::cout << "Commande inconnue : " << command << std::endl;
}
void handleNick(const std::string& params)
{
	std::cout << "Handling NICK command with params: " << params << std::endl;
}

void handleJoin(const std::string& params)
{
	std::cout << "Handling JOIN command with params: " << params << std::endl;
}
