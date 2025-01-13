/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 16:23:54 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/13 18:42:31 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include "ChanMode.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "PrivMsg.hpp"
#include "utils.hpp"
#include "serverExceptions.hpp"
#include "Rpl.hpp"
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

Channel::Channel(Server &server, const std::string &name, const std::string &passwd) : m_cMode(passwd), m_name(name), m_serv(&server), m_channelType(Public)
{
	if (!parseChannelName(name))
		throw serverExceptions(476);
}

Channel::~Channel	(void)
{
	m_serv->sendLog("DEBUG : Channl destructor called : " + this->getName());
	for (size_t i = 0; i < m_vClients.size(); i++)
		m_vClients[i]->leaveChannel(*this);
	for (size_t i = 0; i < m_cMode.getOpClient().size(); i++)
		m_cMode.getOpClient()[i]->leaveOP(*this);
}

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
	std::cout << "test " << &client << std::endl;
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
	this->m_serv->sendLog("Adding " + client.getNickname() + " to " + this->getName() + " channel");
	client.addChannel(*this);
	std::cout << client.m_vChannels[0]->getName() << std::endl;
	if (!this->m_cMode.getOpClient().size())
		addOP(client);
	this->m_vClients.push_back(&client);
	this->sendAllJoin(client);
	this->sendTopic(client);
	this->sendClientslist(client);
}

void	Channel::removeClient(const Client & client)
{
	this->getServ()->sendLog("Bug : RemoveClient");
	for (size_t i = 0; i < m_vClients.size(); i++)
	{
		if (m_vClients[i] == &client)
		{
			this->m_serv->sendLog("removing client " + client.getNickname() + " from " + this->getName() + " channel");
			m_vClients.erase(m_vClients.begin() + i);
			if (!m_vClients.size())
				m_serv->deleteChannel(*this);
			return;
		}
	}
}

void	Channel::sendAllMsg(Server *server, Client *client, const std::string & msg, messageMode mode)
{
	for (size_t i = 0; i < m_vClients.size(); i++)
	{
		if (m_vClients[i] != client)
		{
			switch (mode)
			{
				case ePrivMsg:
					server->sendf(m_vClients[i], client, NULL, PRIVMSG, msg.c_str());
					break;
				case eNotice:
					server->sendf(m_vClients[i], client, NULL, NOTICE, msg.c_str());
					break;
				case eQuit:
					server->sendf( m_vClients[i], client, this, QUITMSG, msg.c_str());
					break;
				default:
	  				;
			}
		}
	}
}

void	Channel::sendAllMode(bool status, const std::string &modeLetter)
{
	std::string statuschar;
	if (status)
		statuschar = "+";
	else
		statuschar = '-';
	for (size_t i = 0; i < this->m_vClients.size(); i++)
		this->m_serv->sendf(this->m_vClients[i], NULL, this, MODE + statuschar + modeLetter);
}

void	Channel::sendAllQuit(Client &client, const std::string &message)
{
	if (this->m_vClients.empty())
		std::cout << "client vide" << std::endl;
	printf("%p \n", &this->m_vClients);
	for (std::vector<Client *>::iterator iter = this->m_vClients.begin(); iter != this->m_vClients.end(); iter++)
		this->m_serv->sendf(*iter, &client, this, QUITMSG, message.c_str());
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

void Channel::sendAllJoin(Client &source)
{
	for (size_t i = 0; i < this->m_vClients.size(); i++)
		this->m_serv->sendf(this->m_vClients[i], &source, this, JOIN);
}

void Channel::sendPart(Client &source, const std::string &message)
{
	for (size_t i = 0; i < this->m_vClients.size(); i++)
		this->m_serv->sendf(this->m_vClients[i], &source, this, PART, message.c_str());
}

void Channel::sendKick(Client &source, Client &target, const std::string &message)
{
	for (size_t i = 0; i < this->m_vClients.size(); i++)
		this->m_serv->sendf(this->m_vClients[i], &source, this, KICK, target.getNickname().c_str(), message.c_str());
}

Server *Channel::getServ() { return m_serv; }

//sMODES

std::string Channel::modeToStr()
{
	if (!(this->m_cMode.isInviteOnly() || this->m_cMode.isTopicProtected() || this->m_cMode.isPasswordProtected() || this->m_cMode.isSizeLimited()))
		return "";
	std::string res = "+";
	if (this->m_cMode.isInviteOnly())
		res += "i";
	if (this->m_cMode.isTopicProtected())
		res += "t";
	if (this->m_cMode.isPasswordProtected())
		res += "k";
	if (this->m_cMode.isSizeLimited())
		res += "l" + itoa(this->m_cMode.getLimitSize());
	return res;
}	

ChanMode*	Channel::getMode() { return &m_cMode; }

void Channel::sendTopic(Client &dest)
{
	if (this->m_cMode.getTopic() != "")
		this->m_serv->sendf(&dest, NULL, this, RPL_TOPIC);
}

void Channel::sendAllTopic()
{
	for (size_t i = 0; i < this->m_vClients.size(); i++)
		this->m_serv->sendf(m_vClients[i], NULL, this, TOPIC);
}

void Channel::addOP(Client &client) 
{
	if (!m_cMode.isOP(&client))
	{
		this->m_cMode.addOP(&client);
		client.addOP(*this);
	}
}
