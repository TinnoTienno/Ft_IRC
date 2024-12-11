/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 16:55:26 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/11 13:47:38 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include "Server.hpp"

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

void Client::connect(Server *server)
{
	server->sendMsg(*this, "Welcome to the IRC NETWORK" + this->getNick() + "!" + this->getUser() + "@host", "001");
	server->sendMsg(*this, "Your host is " + server->getName() + ", running version 1.2.3", "002");
	server->sendMsg(*this, "This server was created" + (std::string) " Tue Dec 9 2024 at 12:00:00 GMT", "003");
	server->sendMsg(*this, server->getName() + " 1.2.3 ao", "004");
	server->sendMsg(*this, "CHANMODES=i t, k, o, l : are supported by this server", "005");
}
