/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 16:23:54 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/08 17:05:45 by eschussl         ###   ########.fr       */
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
	std::memset(&this->m_sModes, 0, sizeof(s_channelMode));
	this->m_name = name;
	addClient(client, "");
	addOP(client);
}

Channel::Channel(Server &server, const std::string &name, Client &client, const std::string &passwd)
{
	if (!parseChannelName(name))
		throw serverExceptions(476);
	std::memset(&this->m_sModes, 0, sizeof(s_channelMode));
	this->setPassword(passwd);
	this->m_serv = &server;
	this->m_name = name;
	addClient(client, passwd);
}

Channel::~Channel	(void)
{
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
	this->m_vClients.push_back(&client);
	client.addChannel(*this);
	if (!this->m_sModes.m_vOP.size())
		addOP(client);
	this->sendAllJoin(client);
	this->sendTopic(client);
	this->sendClientslist(client);
}

void	Channel::removeClient(const Client & client)
{
	for (size_t i = 0; i < m_vClients.size(); i++)
	{
		if (m_vClients[i] == &client)
		{
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
					sendf(server, m_vClients[i], PRIVMSG, client->getPrefix().c_str(), msg.c_str());
//					sendf(server, m_vClients[i], PRIVMSG, client->getPrefix().c_str(), this->getName().c_str(), msg.c_str());
					break;
				case eNotice:
					sendf(server, m_vClients[i], NOTICE, client->getPrefix().c_str(), msg.c_str());
					break;
				case eQuit:
					sendf(server, m_vClients[i], QUIT, client->getPrefix().c_str(), msg.c_str());
					break;
				default:
	  				;
			}
		}
	}
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
	sendf(this->m_serv, &dest, RPL_NAMREPLY, this->getSymbol().c_str(), this->getName().c_str(), list.c_str());
	sendf(this->m_serv, &dest, RPL_ENDOFNAMES, this->getName().c_str(), list.c_str());
}

std::string Channel::clientsList()
{
	std::string res = "";
	for (size_t i = 0; i < m_vClients.size(); i++)
	{
		char mode;
		if (this->isClientOP(*m_vClients[i]))
			mode = '@';
		else
			mode = ' ';
		res += mode + m_vClients[i]->getNickname() + " ";
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
		sendf(this->m_serv, this->m_vClients[i], ":%m JOIN :%C", source.getPrefix().c_str(), this->getName().c_str());
}

void Channel::sendPart(Client &client, const std::string &message)
{
	for (size_t i = 0; i < this->m_vClients.size(); i++)
		sendf(this->m_serv, this->m_vClients[i], ":%m PART %C :%m", client.getPrefix().c_str(), this->getName().c_str(), message.c_str());
}

void Channel::sendKick(Client &source, Client &target, const std::string &message)
{
	for (size_t i = 0; i < this->m_vClients.size(); i++)
		sendf(this->m_serv, this->m_vClients[i], ":%P KICK %C %n :%m",source.getPrefix().c_str() ,this->getName().c_str(), target.getNickname().c_str(), message.c_str());
}

Server *Channel::getServ() { return m_serv; }

//sMODES
//Invite
void	Channel::setInviteMode(bool status)
{
	this->m_sModes.i = status;
	if (status)
		for (size_t i = 0; i < this->m_vClients.size(); i++)
			sendf(this->m_serv, this->m_vClients[i], ":%p MODE %c +i", this->getName().c_str());
	else
		for (size_t i = 0; i < this->m_vClients.size(); i++)
			sendf(this->m_serv, this->m_vClients[i], ":%p MODE %c -i", this->getName().c_str());
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
	// sendMode()
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
		sendf(this->m_serv, &dest, RPL_TOPIC, this->getName().c_str(), this->getTopic().c_str());
	else
		sendf(this->m_serv, &dest, RPL_NOTOPIC, this->getName().c_str());
}

void Channel::sendAllTopic()
{
	for (size_t i = 0; i < this->m_vClients.size(); i++)
		sendf(this->m_serv, m_vClients[i], ":%p TOPIC %C :%t", this->getName().c_str(), this->getTopic().c_str());
}

const std::string	Channel::getTopic(void) const {	return this->m_sModes.topic; }

//Password

void	Channel::setPasswordMode(bool status)
{
	this->m_sModes.k = status;
	// sendMode()
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
			this->m_sModes.m_vOP.erase(this->m_sModes.m_vOP.begin() + i);
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
	// sendMode()
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