/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 16:23:54 by aduvilla          #+#    #+#             */
/*   Updated: 2024/12/16 16:24:19 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <stdexcept>
#include "Server.hpp"
#include "Channel.hpp"
#include "utils.hpp"

Channel::Channel	(const std::string &name, Client &client)
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

Channel::Channel	(const std::string &name, Client &client, const std::string &passwd)
{
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
	this->m_vClientKeys.push_back(client.getFD());
}

void Channel::addClient(int clientKey)
{
	if (!this->m_password.empty())
		throw std::runtime_error("Wrong password");
	this->m_vClientKeys.push_back(clientKey);
}

void	Channel::addClient(Client & client, const std::string & passwd)
{
	if (this->m_password != passwd)
		throw std::runtime_error("Wrong password");
	this->m_vClientKeys.push_back(client.getFD());
}

void Channel::addClient(int clientKey, const std::string &passwd)
{
	if (this->m_password != passwd)
		throw std::runtime_error("Wrong password");
	this->m_vClientKeys.push_back(clientKey);
}

void	Channel::removeClient(const Client & client)
{
	for (size_t i = 0; i < m_vClientKeys.size(); i++)
	{
		if (m_vClientKeys[i] == client.getFD())
		{
			m_vClientKeys.erase(m_vClientKeys.begin() + i);
			return;
		}
	}
	throw	std::runtime_error("Remove client: client not found");
}

void Channel::removeClient(int clientKey)
{
	for (size_t i = 0; i < m_vClientKeys.size(); i++)
	{
		if (m_vClientKeys[i] == clientKey)
		{
			m_vClientKeys.erase(m_vClientKeys.begin() + i);
			return;
		}
	}
	throw	std::runtime_error("Remove client: client not found");
}

void Channel::addOP(Client &client)
{
	this->m_vOPKeys.push_back(client.getFD());
}

void Channel::addOP(int clientKey)
{
	this->m_vOPKeys.push_back(clientKey);
}

void Channel::removeOP(Client &client)
{
	for (size_t i = 0; i < m_vOPKeys.size(); i++)
	{
		if (m_vOPKeys[i] == client.getFD())
		{
			m_vOPKeys.erase(m_vOPKeys.begin() + i);
			return;
		}
	}
	throw	std::runtime_error("Remove OP: OP not found");
}

void Channel::removeOP(int clientKey)
{
	for (size_t i = 0; i < m_vOPKeys.size(); i++)
	{
		if (m_vOPKeys[i] == clientKey)
		{
			m_vOPKeys.erase(m_vOPKeys.begin() + i);
			return;
		}
	}
	throw	std::runtime_error("Remove OP: OP not found");
}


void	Channel::sendAllMsg(const std::string & msg, Server *server)
{
	(void) server; // a supprimer !!!!
	for (size_t i = 0; i < m_vClientKeys.size(); i++)
	{
		sendMessage(m_vClientKeys[i], server->getClient(m_vClientKeys[i]).getPrefix(), "PRIVMSG", msg);
	}
}

void	Channel::setTopic(const std::string & topic)
{
	this->m_topic = topic;
}

const std::string	Channel::getName(void) const
{
	return this->m_name;
}

const std::string	Channel::getTopic(void) const
{
	return this->m_topic;
}

void	Channel::setInvite(bool status)
{
	this->m_isInviteOnly = status;
}

bool	Channel::getInvite() const
{
	return this->m_isInviteOnly;
}
