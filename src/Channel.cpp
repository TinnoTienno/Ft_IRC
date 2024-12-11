/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 16:23:54 by aduvilla          #+#    #+#             */
/*   Updated: 2024/12/09 17:37:02 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include <string>
#include <stdexcept>
#include "Server.hpp"

Channel::Channel	(const std::string &name)
{
	if (name.empty())
		throw std::runtime_error("A channel name must not be empty");
	this->m_isInviteOnly = false;
	this->setTopic("");
	this->m_password = "";
	this->m_name = name;
}

Channel::Channel	(const std::string &name, const std::string &passwd)
{
	if (name.empty())
		throw std::runtime_error("A channel name must not be empty");
	this->m_isInviteOnly = false;
	this->setTopic("");
	this->m_password = passwd;
	this->m_name = name;
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

void	Channel::addClient(Client & client)
{
	if (!this->m_password.empty())
		throw std::runtime_error("Wrong password");
	this->m_vClients.push_back(&client);
}

void	Channel::addClient(Client & client, const std::string & passwd)
{
	if (this->m_password != passwd)
		throw std::runtime_error("Wrong password");
	this->m_vClients.push_back(&client);
}

void	Channel::removeClient(const Client & client)
{
	for (size_t i = 0; i < m_vClients.size(); i++)
	{
		if (m_vClients[i] == & client)
		{
			m_vClients.erase(m_vClients.begin() + i);
			return;
		}
	}
	throw	std::runtime_error("Remove client: client not found");
}

void	Channel::sendAllMsg(const std::string & msg, Server *server)
{
	for (size_t i = 0; i < m_vClients.size(); i++)
	{
		server->sendMsg(*m_vClients[i], msg, "");
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
