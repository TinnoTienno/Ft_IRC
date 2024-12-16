/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 16:55:26 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/16 18:35:37 by noda             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include "Server.hpp"
#include "utils.hpp"
#include <iostream>
#include "Channel.hpp"

void Client::kill(const std::string &str) const
{
	std::string msg = "ERROR :Closing Link: " + this->getNick() + "hostname" +" :" + str + "\r\n";
	send(this->getFD(), msg.c_str(), msg.size(), 0);
}

Client::Client() {
	m_authentified = false;
	m_irssiPacket = "";
}

const int& Client::getFD() const { return m_fd; }

void Client::setFD(const int &fd) {	m_fd = fd; }

void Client::setIPadd(const std::string &ipadd) { m_ipAdd = ipadd; }

void	Client::setHost(struct sockaddr * addr, Server * server)
{
	char	host[NI_MAXHOST];
	int		res = getnameinfo(addr, sizeof(&addr), host, sizeof(host), NULL, 0, 0);
	sendMessage(this->getFD(), server->getHostname(), "NOTICE AUTH", "*** No Identd"); 
	sendMessage(this->getFD(), server->getHostname(), "NOTICE AUTH", "*** Looking up your hostname"); 
	if (!res)
	{
		this->m_host = (std::string) host;
		sendMessage(this->getFD(), server->getHostname(), "NOTICE AUTH", "*** Found your hostname"); 
	}
	else
	{
		this->m_host = this->m_ipAdd;
		sendMessage(this->getFD(), server->getHostname(), "NOTICE AUTH", "*** Hostname not found, IP instead"); 
	}
}

const bool& Client::getAuth() const { return m_authentified; }

void Client::setAuth(const bool &is) { this->m_authentified = is; }

void Client::addPacket(const std::string &packet) {	this->m_irssiPacket += packet; }

std::string Client::getPacket()
{
	std::string tmp = this->m_irssiPacket;
	this->m_irssiPacket = "";
	return tmp;
}

const std::string& Client::getNick() const { return this->m_nick; }

void Client::setNick(const std::string &nick) {	this->m_nick = nick; }

const std::string& Client::getUser() const { return this->m_user; }

void Client::setUser(const std::string &user) {	this->m_user = user; }

const std::string& Client::getReal() const { return this->m_realname; } 

void Client::setReal(const std::string &real) { this->m_realname = real; }
		
Client::~Client()
{
	// close(m_fd);
	// for (size_t i = 0; i < m_vChannels.size(); i++)
		// m_vChannels[i]->removeClient(, *this);// We have to fix this dont know how tho
	for (size_t i = 0; i < m_vChannels.size(); i++)
		m_vChannels[i]->removeOP(*this);
	// std::cout << "client is dead" << std::endl;
};

std::string	Client::getPrefix() const
{
	std::string prefix = this->getNick() + "!" + this->getUser() + "@" + this->m_host;
	return prefix;
}

void Client::connect(Server *server)
{
	std::string y = "10"; // nombre de Operators online
	std::string	msg = "Welcome to the ft_IRC NETWORK " + this->getPrefix();
	sendMessage(this->getFD(), server->getHostname(), "001 " + this->getNick(), msg); 
	msg = "Your host is " + server->getHostname() + ", running version 1.2.3";
	sendMessage(this->getFD(), server->getHostname(), "002 " + this->getNick(), msg); 
	msg = "This server was created " + getTime();
//	msg = "This server was created" + (std::string) " Tue Dec 9 2024 at 12:00:00 GMT";
	sendMessage(this->getFD(), server->getHostname(), "003 " + this->getNick(), msg); 
	msg = server->getHostname() + " 1.2.3 itkOl";
	sendMessage(this->getFD(), server->getHostname(), "004 " + this->getNick(), msg); 
	msg = "CHANMODES=i t, k, o, l : are supported by this server";
	sendMessage(this->getFD(), server->getHostname(), "005 " + this->getNick(), msg);
	msg = "There are " + server->getUserNumber() + " users on 1 server";
	sendMessage(this->getFD(), server->getHostname(), "251 " + this->getNick(), msg); 
	msg = y + " :IRC Operators online";
	sendMessage(this->getFD(), server->getHostname(), "252 " + this->getNick(), msg); 
	msg = server->getChannelNumber() + " :channels formed";
	sendMessage(this->getFD(), server->getHostname(), "254 " + this->getNick(), msg); 
	msg = server->getHostname() + " Message of the Day -";
	sendMessage(this->getFD(), server->getHostname(), "375 " + this->getNick(), msg); 
	std::string	motd[] = {"-        Welcome to",
							"-",
							"-        " + server->getHostname(),
							"-",
							"-        * Host.....: " + server->getHostname(),
							"-        * Port.....: " + server->getPort(),
							"-",
							"-        Welcome to our 42 irc project",
							"-        by eschussl and aduvilla"};
	for (size_t i = 0; i < sizeof(motd) / sizeof(motd[0]); i++)
	{
		sendMessage(this->getFD(), server->getHostname(), "372 " + this->getNick(), motd[i]); 
	}
	msg = server->getHostname() + " End of /MOTD command.";
	sendMessage(this->getFD(), server->getHostname(), "376 " + this->getNick(), msg); 
}

void Client::addChannel(Channel &channel)
{
	this->m_vChannels.push_back(&channel);
	std::cout << "Channel " << channel.getName() << " was added to " << this->getNick() << "'s channels list" << std::endl;
}

void Client::addOP(Channel &channel)
{	
	this->m_OpChannels.push_back(&channel);
	std::cout << "Channel " << channel.getName() << " was added to " << this->getNick() << "'s channels OP list" << std::endl;
}