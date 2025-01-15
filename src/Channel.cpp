/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 16:23:54 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/15 16:01:35 by noda             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstddef>
#include <string>
#include "ChanMode.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "PrivMsg.hpp"
#include "utils.hpp"
#include "serverExceptions.hpp"
#include "Numerics.hpp"
#include <iostream>
#include <cstring>
#include "Join.hpp"
#include "Part.hpp"
#include "Kick.hpp"
#include "Mode.hpp"
#include "Topic.hpp"
#include "Notice.hpp"
#include "Quit.hpp"
#include <stdio.h>
#include <vector>

bool Channel::parseChannelName(const std::string &channelName)
{
	if (charIsNot(channelName[0], "&#+!")|| channelName.size() >= CHANNEL_NAME_MAX_LENGTH || channelName.size() <= CHANNEL_NAME_MIN_LENGTH)
		return false;
	if (channelName.find_first_of(", :") != channelName.npos)
		return false;
	return true;
}

Channel::Channel(Server &server, const std::string &name) : m_cMode(), m_name(name), m_serv(&server), m_channelType(Public)
{
	if (!parseChannelName(name))
		throw serverExceptions(476);
}

Channel::Channel(Server &server, const std::string &name, const std::string &passwd) : m_name(name), m_serv(&server), m_channelType(Public)
{
	if (!parseChannelName(name))
		throw serverExceptions(476);
	this->getMode()->setPasswordProtected(true);
	this->getMode()->setPassword(passwd);
}

Channel::~Channel	(void)
{
}

Channel::Channel(const Channel & copy) : m_vClients(copy.m_vClients), m_cMode(copy.m_cMode), m_name(copy.m_name), m_serv(copy.m_serv), m_channelType(copy.m_channelType)
{
}

Channel&	Channel::operator=(const Channel & rhs)
{
	if (this == &rhs)
		return *this;
	m_cMode = rhs.m_cMode;
	m_name = rhs.m_name;
	m_serv = rhs.m_serv;
	m_channelType = rhs.m_channelType;
	m_vClients = rhs.m_vClients;
	return *this;
}

void	Channel::cleanClient()
{
	std::cout << "Bug cleanclient" << std::endl;
	for (size_t i = 0; i < m_vClients.size(); i++)
		m_vClients[i]->leaveChannel(*this);
	for (size_t i = 0; i < m_cMode.getOpClient().size(); i++)
		m_cMode.getOpClient()[i]->leaveOP(*this);
}

bool	Channel::isEmpty() const { return m_vClients.empty(); }

bool	Channel::isJoinable(Client &client)
{
	if (this->m_cMode.isBanned(&client))
		return false;
	else if (this->getClient(&client))
		return true;
	else if ((!this->m_cMode.isPasswordProtected() || this->m_cMode.isPasswordValid("")) && (!this->m_cMode.isInviteOnly() || this->m_cMode.isInvited(client)))
		return true;
	return false;
}

void	Channel::addClient(Client &client, const std::string &passwd)
{
	if (this->getClient(&client)) // client already in channel
		throw serverExceptions(405);
	if (m_cMode.isPasswordProtected() && !m_cMode.isPasswordValid(passwd)) //passwordMode on and passwd is not valid
		throw serverExceptions(475);
	if (client.getChannelsCount() == MAX_CHANNEL_JOINED - 1)
		throw serverExceptions(405);
	if (this->m_cMode.isInviteOnly() && !this->m_cMode.isInvited(client))
		throw serverExceptions(473);
	if (this->m_cMode.isBanned(&client))
		throw serverExceptions(474);
	if (this->m_cMode.isSizeLimited() && m_vClients.size() >= this->m_cMode.getLimitSize())
		throw serverExceptions(471);
	client.addChannel(*this);
	if (this->m_cMode.getOpClient().empty())
		this->addOP(client);
	this->m_vClients.push_back(&client);
	this->sendAllMsg(this->getServ(), &client, "", eJoin);
	this->sendTopic(client);
	this->sendClientslist(client);
}

void	Channel::removeClient(const Client & client)
{
	for (size_t i = 0; i < m_vClients.size(); i++)
	{
		if (m_vClients[i] == &client)
		{
			if (m_vClients.size() == 1)
				m_serv->deleteChannel(*this);
			else
				m_vClients.erase(m_vClients.begin() + i);
			return;
		}
	}
}

void	Channel::sendAllMsg(Server *server, Client *client, const std::string & msg, messageMode mode)
{
	for (std::vector<Client *>::iterator iter = this->m_vClients.begin(); iter != this->m_vClients.end(); ++iter)
	{
		switch (mode)
		{
			case ePrivMsg:
				if (*iter != client || client->isNetCat())
					server->sendf(*iter, client, this, PRIVMSGALL, msg.c_str());
				break;
			case eNotice:
				server->sendf(*iter, client, this, NOTICEALL, msg.c_str());
				break;
			case eQuit:
				if (*iter != client)
					server->sendf( *iter, client, this, QUITMSG, msg.c_str());
				break;
			case eWho:
				if (this->getMode()->isOP(*iter))
					server->sendf(client, *iter, NULL, RPL_WHOREPLY, this->getName().c_str(), (*iter)->getUsername().c_str(), (*iter)->getHostName().c_str(), "H@", (*iter)->getRealname().c_str());
				else
					server->sendf(client, *iter, NULL, RPL_WHOREPLY, this->getName().c_str(), (*iter)->getUsername().c_str(), (*iter)->getHostName().c_str(), "H", (*iter)->getRealname().c_str());
				break;
			case eJoin:
				server->sendf(*iter, client, this, JOIN);
				break;
			case ePart:
				server->sendf(*iter, client, this, PART, msg.c_str());
				break;
			case eMode:
				server->sendf(*iter, NULL, this, MODE, msg.c_str());
				break;
			case eTopic:
				server->sendf(*iter, NULL, this, TOPIC);
				break;
			default:
				;
		}
	}
	if (mode == eWho)
		server->sendf(client, NULL, NULL, RPL_ENDOFWHO, msg.c_str());
}

void Channel::sendKick(Client &source, Client &target, const std::string &message)
{
	for (size_t i = 0; i < this->m_vClients.size(); i++)
		this->m_serv->sendf(this->m_vClients[i], &source, this, KICK, target.getNickname().c_str(), message.c_str());
}

const std::string	Channel::getName(void) const { return this->m_name; }
		
Client *Channel::getClient(Client *client)
{
	for (size_t i = 0; i < m_vClients.size(); i++)
		if (m_vClients[i] == client)
			return m_vClients[i];
	return NULL;
}

Client *Channel::getClient(const std::string &nickname)
{
	for (size_t i = 0; i < m_vClients.size(); i++)
		if (strCompareNoCase(m_vClients[i]->getNickname(), nickname))
			return m_vClients[i];
	return NULL;
}

void Channel::sendClientslist(Client &dest)
{
	std::string list = this->clientsList();
	m_serv->sendf(&dest, NULL, this, RPL_NAMREPLY, this->getSymbol().c_str(), list.c_str());
	m_serv->sendf(&dest, NULL, this, RPL_ENDOFNAMES, list.c_str());
}

std::string Channel::clientsList()
{
	std::string res = "";
	for (size_t i = 0; i < m_vClients.size(); i++)
	{
		if (this->m_cMode.isOP(m_vClients[i]))
			res += '@' + m_vClients[i]->getNickname() + " ";
		else
			res += m_vClients[i]->getNickname() + " ";
	}
	return res;
}

std::string Channel::getSymbol()
{
	switch (m_channelType)
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


Server *Channel::getServ() { return m_serv; }

//MODES

ChanMode*	Channel::getMode() { return &m_cMode; }

void Channel::sendTopic(Client &dest)
{
	if (this->m_cMode.getTopic() != "")
		this->m_serv->sendf(&dest, NULL, this, RPL_TOPIC);
}


void Channel::addOP(Client &client) 
{
	if (!m_cMode.isOP(&client))
	{
		this->m_cMode.addOP(&client);
		client.addOP(*this);
	}
}
