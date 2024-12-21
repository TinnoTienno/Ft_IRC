/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 16:23:54 by aduvilla          #+#    #+#             */
/*   Updated: 2024/12/21 09:43:29 by noda             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <stdexcept>
#include "Client.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "PrivMsg.hpp"
#include "utils.hpp"
#include <climits>
#include "serverExceptions.hpp"
#include "Rpl.hpp"
#include <iostream>

bool Channel::parseChannelName(const std::string &channelName)
{
	
	if (charIsNot(channelName[0], "&#+!")|| channelName.size() >= CHANNEL_NAME_MAX_LENGTH || channelName.size() <= CHANNEL_NAME_MIN_LENGTH)
		return false;
	if (channelName.find_first_of(", :") != channelName.npos)
		return false;
	return true;
}

Channel::Channel(Server &server, const std::string &name, Client &client)
{
	if (!parseChannelName(name))
		throw serverExceptions(476);
	this->m_serv = &server;
	this->m_isInviteOnly = false;
	this->setTopic("");
	this->m_password = "";
	this->m_name = name;
	this->m_channelMode = Public;
	addClient(client, Founder);
	addOP(client);
}

Channel::Channel(Server &server, const std::string &name, Client &client, const std::string &passwd)
{
	if (!parseChannelName(name))
		throw serverExceptions(476);
	this->m_serv = &server;
	std::cout << "server : " << this->m_serv->getHostname() << std::endl;
	this->m_isInviteOnly = false;
	this->setTopic("");
	this->m_password = passwd;
	this->m_name = name;
	this->m_channelMode = Public;
	addClient(client, passwd, Founder);
	addOP(client);
}


Channel::~Channel	(void)
{
}

void	Channel::addClient(Client &client, Mode clientMode)
{
	if (this->getClient(&client))
		throw serverExceptions(405);
	if (this->m_password != "")
		throw serverExceptions(475);
	if (client.getChannelsCount() == MAX_CHANNEL_JOINED + 1)
		throw serverExceptions(405);
	if (this->getInvite())
		throw serverExceptions(473);
	if (this->getBanned(&client))
		throw serverExceptions(474);
	s_clientPair res = {&client, clientMode} ;
	this->m_vClients.push_back(res);
	if (!m_vOP.size())
		addOP(client);
	this->sendJoin(client);
	this->sendTopic(&client);
	this->sendClientslist(client);
}

void	Channel::addClient(Client &client, const std::string & passwd, Mode clientMode)
{
	if (this->m_password != passwd)
		throw serverExceptions(475);
	if (this->getClient(&client))
		throw serverExceptions(405);
	if (client.getChannelsCount() == MAX_CHANNEL_JOINED + 1)
		throw serverExceptions(405);
	if (this->getInvite())
		throw serverExceptions(473);
	if (this->getBanned(&client))
		throw serverExceptions(474);
	s_clientPair res = {&client, clientMode} ;
	this->m_vClients.push_back(res);
	this->sendJoin(client);
	this->sendTopic(&client);
	this->sendClientslist(client);
}

void	Channel::removeClient(Server &server, const Client & client)
{
	for (size_t i = 0; i < m_vClients.size(); i++)
	{
		if (m_vClients[i].client == &client)
		{
			m_vClients.erase(m_vClients.begin() + i);
			if (!m_vClients.size())
				server.deleteChannel(*this);
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

void	Channel::sendAllMsg(Server &server, Client *client, const std::string & msg)
{
	for (size_t i = 0; i < m_vClients.size(); i++)
		if (m_vClients[i].client != client)
			sendf(&server, m_vClients[i].client, PRIVMSG, client->getPrefix().c_str(), this->getName().c_str(), msg.c_str());
//		sendMessage(m_vClients[i].client->getFD(), m_vClients[i].client->getPrefix(), "PRIVMSG", msg);
}

void	Channel::setTopic(const std::string & topic) { this->m_topic = topic; }

const std::string	Channel::getName(void) const { return this->m_name; }

const std::string	Channel::getTopic(void) const {	return this->m_topic; }

void	Channel::setInvite(bool status) { this->m_isInviteOnly = status; }

bool	Channel::getInvite() const { return this->m_isInviteOnly; }

int Channel::getID() const { return this->m_ID; }

void Channel::setPassword(const std::string &passwd) { m_password = passwd; }

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
		if (m_vClients[i].client == client)
			return m_vClients[i].client;
	return NULL;
}

void Channel::sendTopic(Client *client)
{
	if (this->getTopic() != "")
		sendf(this->m_serv, client, RPL_TOPIC, this->getName().c_str(), this->getTopic().c_str());
	// else
	// 	sendf(this->m_serv, client, RPL_NOTOPIC, this->getName().c_str());
}

void Channel::sendClientslist(Client &dest)
{
	std::string list = this->clientsList();
	std::cout << list << std::endl;
	if (this->m_serv)
		std::cout << "m_Serv is in" << std::endl;
	sendf(this->m_serv, &dest, RPL_NAMREPLY, this->getSymbol().c_str(), this->getName().c_str(), list.c_str());
	sendf(this->m_serv, &dest, RPL_ENDOFNAMES, this->getName().c_str(), list.c_str());
}

std::string Channel::clientsList()
{
	std::string res = "";
	for (size_t i = 0; i < m_vClients.size(); i++)
		res += getMode(m_vClients[i].mode) + m_vClients[i].client->getNick() + " ";
	return res;
}

std::string Channel::getSymbol()
{
	switch (m_channelMode)
	{
	case Public :
		return " =";
	case Secret :
		return " @";
	case Private :
		return " *";
	default :
		return "";
	}
}

void Channel::sendJoin(Client &client)
{
	for (size_t i = 0; i < this->m_vClients.size(); i++)
		sendf(this->m_serv, this->m_vClients[i].client, ":%m JOIN :%C", client.getPrefix().c_str(), this->getName().c_str());
}

