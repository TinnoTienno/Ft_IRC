/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 16:23:54 by aduvilla          #+#    #+#             */
/*   Updated: 2024/12/16 18:16:08 by noda             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <stdexcept>
#include "Server.hpp"
#include "Channel.hpp"
#include "utils.hpp"
#include <climits>

Channel::Channel(const std::string &name, Client &client)
{
	if (name.empty())
		throw std::runtime_error("A channel name must not be empty");
	this->m_isInviteOnly = false;
	this->setTopic("");
	this->m_password = "";
	this->m_name = name;
	addClient(client);
	addOP(client);
}

Channel::Channel(const std::string &name, Client &client, const std::string &passwd)
{
	static int ID = 0;
	this->m_ID = --ID;
	if (name.empty())
		throw std::runtime_error("A channel name must not be empty");
	this->m_isInviteOnly = false;
	this->setTopic("");
	this->m_password = passwd;
	this->m_name = name;
	addClient(client, passwd);
	addOP(client);
}

Channel::Channel	(Channel const & src)
{
	*this = src;
}

Channel&	Channel::operator=(Channel const & rhs)
{
	if (this != &rhs)
	{
		this->m_isInviteOnly = rhs.m_isInviteOnly;
		this->setTopic(rhs.getTopic());
		this->m_password = rhs.m_password;
		this->m_name = rhs.m_name;
	}
	return *this;
}

Channel::~Channel	(void)
{
}

void	Channel::addClient(Client &client)
{
	if (!this->m_password.empty())
		throw std::runtime_error("Wrong password");
	this->m_vClients.push_back(&client);
	if (!m_vOP.size())
		addOP(client);
	this->sendJoin(client);
}

void	Channel::addClient(Client &client, const std::string & passwd)
{
	if (this->m_password != passwd)
		throw std::runtime_error("Wrong password");
	this->m_vClients.push_back(&client);
	this->sendJoin(client);
}

void	Channel::removeClient(Server *server, const Client & client)
{
	for (size_t i = 0; i < m_vClients.size(); i++)
	{
		if (m_vClients[i] == &client)
		{
			m_vClients.erase(m_vClients.begin() + i);
			if (!m_vClients.size())
				server->deleteChannel(*this);
			return;
		}
	}
	throw	std::runtime_error("Remove client: client not found");
}

void Channel::addOP(Client &client) { this->m_vOP.push_back(&client); }

void Channel::removeOP(Client &client)
{
	for (size_t i = 0; i < m_vOP.size(); i++)
	{
		if (m_vOP[i] == &client)
		{
			m_vOP.erase(m_vOP.begin() + i);
			return;
		}
	}
	throw	std::runtime_error("Remove OP: OP not found");
}

void	Channel::sendAllMsg(const std::string & msg)
{
	for (size_t i = 0; i < m_vClients.size(); i++)
		sendMessage(m_vClients[i]->getFD(), m_vClients[i]->getPrefix(), "PRIVMSG", msg);
}

void	Channel::setTopic(const std::string & topic) { this->m_topic = topic; }

const std::string	Channel::getName(void) const { return this->m_name; }

const std::string	Channel::getTopic(void) const {	return this->m_topic; }

void	Channel::setInvite(bool status) { this->m_isInviteOnly = status; }

bool	Channel::getInvite() const { return this->m_isInviteOnly; }

int Channel::getID() const { return this->m_ID; }

void Channel::setPassword(const std::string &passwd) { m_password = passwd; }

void Channel::sendJoin(const Client &client)
{
	for (size_t i = 0; i < m_vClients.size(); i++)
		sendMessage(m_vClients[i]->getFD(), client.getPrefix(), "JOIN ", this->getName());
}