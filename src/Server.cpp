/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 15:33:51 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/09 18:36:53 by eschussl         ###   ########.fr       */
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
#include "Join.hpp"
#include "Nick.hpp"
#include "Channel.hpp"
#include "UserHost.hpp"
#include <netdb.h>


#define RED "\e[1;31m" //-> for red color
#define WHI "\e[0;37m" //-> for white color
#define GRE "\e[1;32m" //-> for green color
#define YEL "\e[1;33m" //-> for yellow color

bool Server::m_signal = false;

Server::Server(const std::string &name, const std::string &pass) : m_pass(pass), m_name(name.substr(2, name.npos))
{ 
	m_serverSocketFd = -1;
	char hostname[256];
	struct hostent *host;
	struct in_addr **addr_list;
	// BuildCommandMap();
	// m_CommandHandlers["JOIN"] = Join::execute();
	gethostname(hostname, sizeof(hostname));
	host = gethostbyname(hostname);
	addr_list = (struct in_addr **) host->h_addr_list;
	m_serverHostname = inet_ntoa(*addr_list[0]);
}



Server::~Server()
{
	for (size_t i = 0; i < m_vFds.size(); i++)
		ClearClients(m_vFds[i].fd);
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
		if ((poll(&m_vFds[0], m_vFds.size(), -1) == -1) && m_signal == false)
			throw(std::runtime_error("Poll failed"));
		for (size_t i = 0; i < m_vFds.size(); i++)
		{
			if (m_vFds[i].revents & POLLIN)
			{
				if (m_vFds[i].fd == m_serverSocketFd)
				{
					AcceptNewClient();
				}
				else
				{
					ReceiveNewData(m_vFds[i].fd);
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
	m_vFds.push_back(Poll);
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
	m_vClients.push_back(client);
	m_vFds.push_back(newPoll);
	std::cout << GRE << "Client <" << incoFd << "> Connected" << WHI << std::endl;
}
bool Server::checkNick(Client &client, const std::string &buffer)
{
	size_t nick = buffer.find("NICK");
	nick = buffer.find(' ', nick) + 1;
	size_t nick2 = buffer.find(13, nick);
	std::string  nickname = buffer.substr(nick, nick2 - nick);
	std::cout << "nick |" << nickname << "|" << std::endl;
	client.setNick(nickname);
	for (size_t i = 0; i < m_vClients.size(); i++)
	{
		if (m_vClients[i].getNick() == nickname && &m_vClients[i] != &client)
		{
			std::cout << RED << "Client <" << client.getFD() << "> nickname already used" << WHI << std::endl;
			client.kill("nickname already taken");
			ClearClients(client.getFD());
			return 0;
		}
	}
	return 1;
}

bool Server::checkUser(Client &client, const std::string &buffer)
{
	size_t user = buffer.find("USER");
	user = buffer.find(' ', user) + 1;
	size_t user2 = buffer.find(' ', user);
	std::string  username = buffer.substr(user, user2 - user);
	std::cout << "username |" << username << "|" << std::endl;
	client.setUser(username);
	return 1;
}

bool Server::checkAuth(Client &client, const std::string &buffer)
{

	if (client.getAuth() == 1)
		return 1;
	if (buffer.find("USER") == buffer.npos)
	    return 0;
	std::cout << "checkAuth buffer " << buffer << std::endl;
	if (!checkNick(client, buffer))
		return 0;
	if (!checkUser(client, buffer))
		return 0;
	size_t pass = buffer.find("PASS");
	if (pass == buffer.npos)
	{
	    std::cout << RED << "Client <" << client.getFD() << "> has not set a password" << WHI << std::endl;
		this->sendMsg(client, "please send a password", "");
		return 0;
	}
	pass = buffer.find(' ', pass) + 1;
	size_t pass2 = buffer.find(13, pass);
	std::string passwd = buffer.substr(pass, pass2 - pass);
	std::cout << "pass : |" << passwd << "|" << std::endl;
	if (!passwd.compare(m_pass))
	{
		client.setAuth(true);
		std::cout << GRE << "Client <" << client.getFD() << "> is now authentified" << WHI << std::endl;
		client.connect(this);
		this->sendMsg(client, "Authentified", "");
		return 0;
	}
	std::cout << RED << "Client <" << client.getFD() << "> has a wrong password" << WHI << std::endl;
	client.kill("password is wrong");
	ClearClients(client.getFD());
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
		size_t i = 0;
		for (; i < m_vClients.size(); i++)
		{
			if (m_vClients[i].getFD() == fd)
				break ;
		}
		buff[bytes] = '\0';
		std::string buffer = buff;
		if (checkAuth(m_vClients[i], buffer))
		{
			size_t j = 0;
			for (size_t i = 0; i < buffer.size(); i = j)
			{
				j = buffer.find("\r", i + 1);
				parseCommand(buffer.substr(i, j), m_vClients[i]);
			}
		}
			// std::cout << YEL << "Client <" << fd << "> Data: " << WHI << buff;
		
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

const std::string Server::getName() const { return m_name; }

const std::string Server::getHostname() const { return m_serverHostname; }

void Server::sendMsg(Client &client, const std::string &str, const std::string &code) const
{
	std::string msg;
	msg = ":irc." + this->getName() + (std::string) " " + code + " " + client.getNick() + " :" + str + "\r\n";
	// std::string msg = ":" + client.getNick() + "!" + client.getUser() + "@" + this->getHostname() + (std::string) " PRIVMSG " + client.getNick() + " :" + str + "\r\n";
	// std::string msg = "PRIVMSG " + client.getNick() + " :" + str + "\r\n";
	send(client.getFD(), msg.c_str(), msg.size(), 0);
	std::cout << "sent : |" << msg << "|" << std::endl;
}

bool Server::findNick(const std::string &nickname)
{
	for (size_t i = 0; i < m_vClients.size(); i++)
	{
		if (nickname == m_vClients[i].getNick())
			return 1;
	}
	return 0;
}