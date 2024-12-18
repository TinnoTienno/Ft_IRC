/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 16:23:54 by aduvilla          #+#    #+#             */
/*   Updated: 2024/12/18 18:14:41 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <stdexcept>
#include "Server.hpp"
#include "Channel.hpp"
#include "utils.hpp"
#include <climits>
#include "serverExceptions.hpp"
#include "Rpl.hpp"

bool Channel::parseChannelName(const std::string &channelName)
{
	if (charIsNot(channelName[0], "&#+!") || channelName.size() >= CHANNEL_NAME_MAX_LENGTH || channelName.size() <= CHANNEL_NAME_MIN_LENGTH)
		return false;
	if (channelName.find_first_of(", :") != channelName.npos)
		return false;
	return true;
}

Channel::Channel(Server *server, const std::string &name, Client &client)
{
	if (!parseChannelName(name))
		throw serverExceptions(476);
	this->m_serv = server;
	this->m_isInviteOnly = false;
	this->setTopic("");
	this->m_password = "";
	this->m_name = name;
	addClient(client);
	addOP(client);
}

Channel::Channel(Server *server, const std::string &name, Client &client, const std::string &passwd)
{
	if (!parseChannelName(name))
		throw serverExceptions(476);
	this->m_serv = server;
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
	if (this->getClient(&client))
		throw serverExceptions(405);
	if (!this->m_password.empty())
		throw serverExceptions(475);
	if (client.getChannelsCount() == MAX_CHANNEL_JOINED + 1)
		throw serverExceptions(405);
	if (this->getBanned(&client))
		throw serverExceptions(474);
	this->m_vClients.push_back(&client);
	if (!m_vOP.size())
		addOP(client);
	this->sendJoin(client);
	this->sendTopic(client);
}

void	Channel::addClient(Client &client, const std::string & passwd)
{
	if (this->m_password != passwd)
		throw serverExceptions(475);
	if (this->getClient(&client))
		throw serverExceptions(405);
	if (client.getChannelsCount() == MAX_CHANNEL_JOINED + 1)
		throw serverExceptions(405);
	if (this->getBanned(&client))
		throw serverExceptions(474);
	this->m_vClients.push_back(&client);
	this->sendJoin(client);
	this->sendTopic(client);
	this->sendClientslist(client);
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

Client *Channel::getBanned(Client *client)
{
	for (size_t i = 0; i < m_vBans.size(); i++)
		if (m_vBans[i] == client)
			return m_vBans[i];
	return NULL;
}

Client *Channel::getClient(Client *client)
{
	for (size_t i = 0; i < m_vClients.size(); i++)
		if (m_vClients[i] == client)
			return m_vClients[i];
	return NULL;
}

void Channel::sendTopic(Client &client)
{
	if (this->getTopic() != "")
		sendf(this->m_serv, &client, RPL_TOPIC, this->getName(), this->getTopic());
	else
		sendf(this->m_serv, &client, RPL_NOTOPIC, this->getName());
}

void Channel::sendClientslist(Client &dest)
{
	std::string list = this->clientsList();
	sendf(this->m_serv, &dest, RPL_NAMREPLY, this->getsymbol(), this->getName(), list);
}

std::string Channel::clientsList()
{
	std::string res = "";
	for (size_t i = 0; i < m_vClients.size(); i++)
	{
		res += m_vClients[i].g
	}
}