/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 16:55:26 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/13 15:45:24 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <unistd.h>
#include <sys/socket.h>
#include "Server.hpp"
#include "utils.hpp"

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

const bool& Client::getAuth() const { return m_authentified; }

void Client::setAuth(const bool &is)
{
	this->m_authentified = is;
}

void Client::addPacket(const std::string &packet)
{
	this->m_irssiPacket += packet;
}

std::string Client::getPacket()
{
	std::string tmp = this->m_irssiPacket;
	this->m_irssiPacket = "";
	return tmp;
}

const std::string& Client::getNick() const
{
	return this->m_nick;
}

void Client::setNick(const std::string &nick)
{
	this->m_nick = nick;
}

const std::string& Client::getUser() const { return this->m_user; }
void Client::setUser(const std::string &user) {	this->m_user = user; }

Client::~Client()
{
	// close(m_fd);
	// std::cout << "client is dead" << std::endl;
};

std::string	Client::getPrefix(const std::string & host) const
{
	std::string prefix = this->getNick() + "!" + this->getUser() + "@" + host;
	return prefix;
}

void Client::connect(Server *server)
{
	// :irc.server.com NOTICE AUTH :*** Looking up your hostname...
	// :irc.server.com NOTICE AUTH :*** Checking Ident
	// :irc.server.com NOTICE AUTH :*** Found tour hostname
	// :irc.server.com NOTICE AUTH :*** No Ident response
	std::string x = "10"; // nombre de users
	std::string y = "10"; // nombre de Operators online
	std::string z = "10"; // nombre de channels online
	std::string	port = "6667"; // prévoir fn getPort
	std::string	msg = "Welcome to the ft_IRC NETWORK" + this->getPrefix(server->getHostname());
	sendMessage(this->getFD(), server->getHostname(), "001 " + this->getNick(), msg); 
	msg = "Your host is " + server->getHostname() + ", running version 1.2.3";
	sendMessage(this->getFD(), server->getHostname(), "002 " + this->getNick(), msg); 
	sendMessage(this->getFD(), server->getHostname(), "003 " + this->getNick(), "This server was created" + (std::string) " Tue Dec 9 2024 at 12:00:00 GMT");
	sendMessage(this->getFD(), server->getHostname(), "004 " + this->getNick(), server->getHostname() + " 1.2.3 itkOl");
	sendMessage(this->getFD(), server->getHostname(), "005 " + this->getNick(), "CHANMODES=i t, k, o, l : are supported by this server");
	msg = "There are " + x + " number of users on 1 server";
	sendMessage(this->getFD(), server->getHostname(), "251 " + this->getNick(), msg); 
	msg = y + " :IRC Operators online";
	sendMessage(this->getFD(), server->getHostname(), "252 " + this->getNick(), msg); 
	msg = z + " :channels formed";
	sendMessage(this->getFD(), server->getHostname(), "254 " + this->getNick(), msg); 
	msg = server->getHostname() + "Message of the Day -";
	sendMessage(this->getFD(), server->getHostname(), "375 " + this->getNick(), msg); 
	std::string	motd[] = {"-			Welcome to",
							"-",
							"-			" + server->getHostname(),
							"-",
							"-			* Host.....: " + server->getHostname(),
							"-			* Port.....: " + port,
							"-",
							"-			Welcome to our 42 irc project",
							"-			by eschussl and aduvilla"};
	for (size_t i = 0; i < sizeof(motd) / sizeof(motd[0]); i++)
	{
		sendMessage(this->getFD(), server->getHostname(), "372 " + this->getNick(), motd[i]); 
	}
}
