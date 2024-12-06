/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 16:55:26 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/06 15:37:48 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <unistd.h>
#include <iostream>
#include <sys/socket.h>


void Client::sendMsg(const std::string &str) const
{
	std::string msg = "PRIVMSG " + this->getNick() + " :" + str + "\r\n";
	send(this->getFD(), msg.c_str(), msg.size(), 0);
}

void Client::kill(const std::string &str) const
{
	std::string msg = "KILL " + this->getNick() + " :" + str + "\n\r";
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


Client::~Client()
{
	// close(m_fd);
	// std::cout << "client is dead" << std::endl;
};