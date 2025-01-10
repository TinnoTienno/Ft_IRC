/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 16:23:54 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/10 18:21:07 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
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

Channel::Channel(Server &server, const std::string &name)
{
	if (!parseChannelName(name))
		throw serverExceptions(476);
	this->m_serv = &server;
	std::memset(&this->m_sModes, 0, sizeof(s_channelMode));
	this->m_channelType = Public;
	this->m_name = name;
}

Channel::Channel(Server &server, const std::string &name, const std::string &passwd)
{
	if (!parseChannelName(name))
		throw serverExceptions(476);
	std::memset(&this->m_sModes, 0, sizeof(s_channelMode));
	this->setPassword(passwd);
	this->m_serv = &server;
	this->m_name = name;
	this->m_channelType = Public;
}

Channel::~Channel	(void)
{
	m_serv->sendLog("DEBUG : Channl destructor called : " + this->getName());
	for (size_t i = 0; i < m_vClients.size(); i++)
		m_vClients[i]->leaveChannel(*this);
	for (size_t i = 0; i < m_sModes.m_vOP.size(); i++)
		m_sModes.m_vOP[i]->leaveOP(*this);
}

bool	Channel::isJoinable(Client &client)
{
	if (this->isBanned(client))
		return false;
	else if (this->getClient(&client))
		return true;
	else if ((!this->getPasswordMode() || this->isPasswordValid("")) && (!this->getInviteMode() || this->isInvited(client)))
		return true;
	return false;
}

void	Channel::addClient(Client &client, const std::string &passwd)
{
	std::cout << "test " << &client << std::endl;
	if (this->getClient(&client))
		throw serverExceptions(405);
	if (getPasswordMode() && !isPasswordValid(passwd))
		throw serverExceptions(475);
	if (client.getChannelsCount() == MAX_CHANNEL_JOINED - 1)
		throw serverExceptions(405);
	if (this->getInviteMode() && this->isInvited(client))
		throw serverExceptions(473);
	if (this->isBanned(client))
		throw serverExceptions(474);
	if (this->getSizeLimitMode() && m_vClients.size() >= this->getSizeLimit())
		throw serverExceptions(471);
	this->m_serv->sendLog("Adding " + client.getNickname() + " to " + this->getName() + " channel");
	client.addChannel(*this);
	std::cout << client.m_vChannels[0]->getName() << std::endl;
	if (!this->m_sModes.m_vOP.size())
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
//					sendf(server, m_vClients[i], PRIVMSG, client->getPrefix().c_str(), this->getName().c_str(), msg.c_str());
					break;
				case eNotice:
					server->sendf(m_vClients[i], client, NULL, NOTICE, msg.c_str());
					break;
				case eQuit:
					server->sendf( m_vClients[i], client, this, QUIT, msg.c_str());
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
		this->m_serv->sendf(*iter, &client, this, QUIT, message.c_str());
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
		if (this->isClientOP(*m_vClients[i]))
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
	if (!(this->m_sModes.i || this->m_sModes.t || this->m_sModes.k || this->m_sModes.l))
		return "";
	std::string res = "+";
	if (this->m_sModes.i)
		res += "i";
	if (this->m_sModes.t)
		res += "t";
	if (this->m_sModes.k)
		res += "k";
	if (this->m_sModes.l)
		res += "l" + itoa(this->getSizeLimit());
	return res;
}	


//Invite
void	Channel::setInviteMode(bool status)
{
	this->m_sModes.i = status;
	this->m_serv->sendLog(this->getName() + "'s invite mode was set to " + itoa(status));
}

bool	Channel::getInviteMode() const { return this->m_sModes.i; }

void	Channel::setInvited(Client &client)
{
	this->m_serv->sendLog("Client : " + client.getNickname() + " was invited to " + this->getName() + " channel");
	// ADD invite sending("%p INVITE %n :%C", client->getNickname(), this->getName())
	this->m_sModes.m_vInvitedHostNames.push_back(client.getPrefix());
}

bool	Channel::isInvited(Client &client)
{
	for (size_t i = 0; i < this->m_sModes.m_vInvitedHostNames.size(); i++)
		if (client.getPrefix() == this->m_sModes.m_vInvitedHostNames[i])
			return true;
	return false;
}

//Topic

void	Channel::setProtectedTopicMode(bool status) 
{
	this->m_sModes.t = status;
	this->m_serv->sendLog(this->getName() + "'s protected topic mode was set to " + itoa(status));
}

bool	Channel::getProtectedTopicMode() const { return this->m_sModes.t; }

void	Channel::setTopic(Client &client, const std::string &topic)
{
	if (this->getProtectedTopicMode() && !this->isClientOP(client))
			throw serverExceptions(482);
	if (topic == "\"\"")
		this->m_sModes.topic = "";
	else
		this->m_sModes.topic = topic;
	m_serv->sendLog("Channel " + this->getName() + "'s topic was set to " + this->m_sModes.topic);
	sendAllTopic();
}

void Channel::sendTopic(Client &dest)
{
	if (this->getTopic() != "")
		this->m_serv->sendf(&dest, NULL, this, RPL_TOPIC);
	else
		this->m_serv->sendf(&dest, NULL, this, RPL_NOTOPIC);
}

void Channel::sendAllTopic()
{
	for (size_t i = 0; i < this->m_vClients.size(); i++)
		this->m_serv->sendf(m_vClients[i], NULL, this, TOPIC);
}

const std::string	Channel::getTopic(void) const {	return this->m_sModes.topic; }

//Password

void	Channel::setPasswordMode(bool status)
{
	this->m_sModes.k = status;
	if (status)
		for (size_t i = 0; i < this->m_vClients.size(); i++)
			this->m_serv->sendf(this->m_vClients[i], NULL, this, MODE + (std::string) "+k");
	else
		for (size_t i = 0; i < this->m_vClients.size(); i++)
			this->m_serv->sendf(this->m_vClients[i], NULL, this, MODE + (std::string) "-k");
	this->m_serv->sendLog(this->getName() + "'s password mode was set to " + itoa(status));
}

bool	Channel::getPasswordMode() { return this->m_sModes.k; }

void	Channel::setPassword(const std::string &passwd)
{
	this->m_sModes.password = passwd;
	// sendMode()
	this->m_serv->sendLog(this->getName() + "'s password was set to " + passwd);
}

bool	Channel::isPasswordValid(const std::string str)
{
	if (str == this->m_sModes.password)
		return true;
	return false;
}

//Operator

void Channel::addOP(Client &client) 
{
	if (!isClientOP(client))
	{
		this->m_sModes.m_vOP.push_back(&client);
		client.addOP(*this);
	}
}

void Channel::removeOP(Client &client)
{
	for (size_t i = 0; i < this->m_sModes.m_vOP.size(); i++)
		if (this->m_sModes.m_vOP[i] == &client)
		{
			this->getServ()->sendLog("Removing OP " + client.getNickname() + " from " + this->getName() + " channel");
			this->m_sModes.m_vOP.erase(this->m_sModes.m_vOP.begin() + i);
		}
	// Message ??
}

bool	Channel::isClientOP(Client &client)	//No more channel founder and such, only OP & normals
{
	for (size_t i = 0; i < this->m_sModes.m_vOP.size(); i++)
		if (this->m_sModes.m_vOP[i] == &client)
			return true;
	return false;
}

//Size limit
void	Channel::setSizeLimitMode(bool status)
{
	this->m_sModes.l = status;
	this->m_serv->sendLog(this->getName() + "'s size limit mode was set to " + itoa(status));
}

bool	Channel::getSizeLimitMode() { return this->m_sModes.l; }

void	Channel::setSizeLimit(unsigned int value) 
{
	if (value == 0)
		return  ;
	this->m_sModes.limitedSize = value;
	this->m_serv->sendLog(this->getName() + "'s size limit was set to " + itoa(value));
}

size_t	Channel::getSizeLimit() { return this->m_sModes.limitedSize; }

//Bans
void	Channel::setBanned(Client &client) { this->m_sModes.m_vBanned.push_back(&client); }

bool	Channel::isBanned(Client &client)
{
	for (size_t i = 0; i < this->m_sModes.m_vBanned.size(); i++)
		if (this->m_sModes.m_vBanned[i] == &client)
			return true;
	return false;
}
