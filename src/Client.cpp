/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 16:55:26 by eschussl          #+#    #+#             */
/*   Updated: 2025/01/10 15:26:26 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <exception>
#include "Client.hpp"
#include <stdexcept>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include "Server.hpp"
#include "utils.hpp"
#include <iostream>
#include "Channel.hpp"
#include "serverExceptions.hpp"

Client::Client() {
	m_authentified = false;
	m_irssiPacket = "";
	m_vChannels.clear();
	m_vOpChannels.clear();
}
		
Client::~Client()
{
	for (size_t i = 0; i < m_vChannels.size(); i++)
		m_vChannels[i]->removeClient(*this);
	for (size_t i = 0; i < m_vOpChannels.size(); i++)
		m_vOpChannels[i]->removeOP(*this);
};

//getters / setters
std::string	Client::getPrefix() const
{
	std::string prefix = this->getNickname() + "!" + this->getUsername() + "@" + this->m_hostname;
	return prefix;
}

void Client::setFD(const int &fd) {	m_fd = fd; }

const int& Client::getFD() const { return m_fd; }

void Client::setIPadd(const std::string &ipadd) { m_ipAdd = ipadd; }

void	Client::setHostname(struct sockaddr *addr, Server &server)
{
	char	host[NI_MAXHOST];
	int		res = getnameinfo(addr, sizeof(&addr), host, sizeof(host), NULL, 0, 0);
	try
	{
		sendMessage(this->getFD(), server, "NOTICE AUTH", "*** No Identd");
		sendMessage(this->getFD(), server, "NOTICE AUTH", "*** Looking up your hostname");
		if (!res)
		{
			this->m_hostname = (std::string) host;
			sendMessage(this->getFD(), server, "NOTICE AUTH", "*** Found your hostname");
				return ;
		}
		else
		{
			this->m_hostname = this->m_ipAdd;
			sendMessage(this->getFD(), server, "NOTICE AUTH", "*** Hostname not found, IP instead");
				return ;
		}
	}
	catch (const std::exception &e)
	{
		server.sendLog("Error: set host: " + (std::string) e.what());
	}
}

void Client::setAuth(const bool &is) { this->m_authentified = is; }

bool Client::getAuth() const { return m_authentified; }

void Client::setUsername(const std::string &username) {	this->m_username = username; }

const std::string& Client::getUsername() const { return this->m_username; }

void Client::setNickname(const std::string &nick) {	this->m_nickname = nick; }

const std::string& Client::getNickname() const { return this->m_nickname; }

void Client::setRealname(const std::string &real) { this->m_realname = real; }

const std::string& Client::getRealname() const { return this->m_realname; }

//packets
void Client::addPacket(const std::string &packet) {	this->m_irssiPacket += packet; }

std::string Client::getPacket()
{
	std::string tmp = this->m_irssiPacket;
	this->m_irssiPacket = "";
	return tmp;
}

//messages
void	Client::sendQuitMsg(const std::string & msg)
{
	// for (size_t i = 0; i < this->m_vChannels.size(); i++)
	for (std::vector<Channel *>::iterator iter = this->m_vChannels.begin(); iter != this->m_vChannels.end(); iter++)
		(*iter)->sendAllQuit(*this, msg);
}

void Client::kill(const std::string &str) const
{
	std::string msg = "ERROR :Closing Link: " + this->getNickname() + "hostname :" + str + "\r\n";
	if (send(this->getFD(), msg.c_str(), msg.size(), 0) != (ssize_t)msg.length())
//		throw std::runtime_error("Failed to send message: " + msg);
		return ;
}

void Client::connect(Server &server)
{
	try
	{
		std::string y = "10"; // nombre de Operators online
		std::string	msg = "Welcome to the ft_IRC NETWORK " + this->getPrefix();
		sendMessage(this->getFD(), server, "001 " + this->getNickname(), msg);
		msg = "Your host is " + server.getHostname() + ", running version 1.2.3";
		sendMessage(this->getFD(), server, "002 " + this->getNickname(), msg);
		msg = "This server was created " + getTime();
		sendMessage(this->getFD(), server, "003 " + this->getNickname(), msg);
		msg = server.getHostname() + " 1.2.3 itkOl";
		sendMessage(this->getFD(), server, "004 " + this->getNickname(), msg);
		msg = "CHANMODES=i t, k, o, l : are supported by this server";
		sendMessage(this->getFD(), server, "005 " + this->getNickname(), msg);
		msg = "There are " + server.getUserNumber() + " users on 1 server";
		sendMessage(this->getFD(), server, "251 " + this->getNickname(), msg);
		msg = y + " :IRC Operators online";
		sendMessage(this->getFD(), server, "252 " + this->getNickname(), msg);
		msg = server.getChannelNumber() + " :channels formed";
		sendMessage(this->getFD(), server, "254 " + this->getNickname(), msg);
		msg = "- " + server.getHostname() + " Message of the Day -";
		sendMessage(this->getFD(), server, "375 " + this->getNickname(), msg);
		std::string	motd[] = {"-        Welcome to,",
								"-",
								"-        " + server.getHostname(),
								"-",
								"-        * Host.....: " + server.getHostname(),
								"-        * Port.....: " + server.getPort(),
								"-",
								"-        Welcome to our 42 irc project",
								"-        by eschussl and aduvilla"};
		for (size_t i = 0; i < sizeof(motd) / sizeof(motd[0]); i++)
			sendMessage(this->getFD(), server, "372 " + this->getNickname(), motd[i]);
		msg = server.getHostname() + " End of /MOTD command.";
		sendMessage(this->getFD(), server, "376 " + this->getNickname(), msg);
	}
	catch (std::exception &e)
	{
		server.sendLog("Error: Connect: " + (std::string) e.what());
	}
}

//vectors
void Client::addChannel(Channel &channel)
{
	this->m_vChannels.push_back(&channel);
	channel.getServ()->sendLog("Channel " + channel.getName() + " was added to " + this->getNickname() + "'s channels list");
}

void Client::leaveChannel(Channel &channel)
{
	for (size_t i = 0; i < m_vChannels.size(); i++)
		if (&channel == m_vChannels[i])
			m_vChannels.erase(m_vChannels.begin() + i);
}

void Client::addOP(Channel &channel)
{	
	this->m_vOpChannels.push_back(&channel);
	channel.getServ()->sendLog("Channel " + channel.getName() + " was added to " + this->getNickname() + "'s channels OP list");
}

void Client::leaveOP(Channel &channel)
{
	for (size_t i = 0; i < m_vOpChannels.size(); i++)
		if (&channel == m_vOpChannels[i])
			m_vOpChannels.erase(m_vOpChannels.begin() + i);
}

size_t Client::getChannelsCount() { return m_vChannels.size(); }
