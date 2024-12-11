/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 16:55:26 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/11 18:14:19 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <unistd.h>
#include <sys/socket.h>
#include "Server.hpp"
#include "utils.hpp"

void Client::kill(const std::string &str) const
{
	std::string msg = "KILL " + this->getNick() + " :" + str + "\r\n";
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
	return this->m_irssiPacket;
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
	sendMessage(this->getFD(), server->getHostname(), "001 " + this->getNick(), "Welcome to the IRC NETWORK" + this->getNick() + "!" + this->getUser() + "@host");
	sendMessage(this->getFD(), server->getHostname(), "002 " + this->getNick(), "Your host is " + server->getHostname() + ", running version 1.2.3");
	sendMessage(this->getFD(), server->getHostname(), "003 " + this->getNick(), "This server was created" + (std::string) " Tue Dec 9 2024 at 12:00:00 GMT");
	sendMessage(this->getFD(), server->getHostname(), "004 " + this->getNick(), server->getHostname() + " 1.2.3 ao");
	sendMessage(this->getFD(), server->getHostname(), "005 " + this->getNick(), "CHANMODES=i t, k, o, l : are supported by this server");
}
