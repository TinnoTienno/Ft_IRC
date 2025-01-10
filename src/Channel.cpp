/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 16:23:54 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/10 14:16:39 by aduvilla         ###   ########.fr       */
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
	this->setTopic(NULL, "");
	this->m_password = "";
	this->m_name = name;
	this->m_channelMode = Public;
	this->m_isChannelSizeLimited = false;
	addClient(client, Operator);
	addOP(client);
}

Channel::Channel(Server &server, const std::string &name, Client &client, const std::string &passwd)
{
	if (!parseChannelName(name))
		throw serverExceptions(476);
	this->m_serv = &server;
	std::cout << "server : " << this->m_serv->getHostname() << std::endl;
	this->m_isInviteOnly = false;
	this->setTopic(NULL, "");
	this->m_password = passwd;
	this->m_name = name;
	this->m_channelMode = Public;
	this->m_isChannelSizeLimited = false;
	addClient(client, passwd, Operator);
	addOP(client);
}


Channel::~Channel	(void)
{
}

bool	Channel::isInvited(Client *client)
{
	for (size_t i = 0; i < this->m_vInvited.size(); i++)
		if (this->m_vInvited[i] == client)
			return true;
	return false;
}

bool	Channel::isOp(Client *client)
{
	for (size_t i = 0; i < this->m_vOP.size(); i++)
		if (this->m_vOP[i] == client)
			return true;
	return false;
}

bool	Channel::isJoinable(Client *client)
{
	if (this->getBanned(client))
		return false;
	else if (this->getClient(client) != NULL)
		return true;
	else if (this->m_password == "" && (!this->m_isInviteOnly || this->isInvited(client)))
			return true;
	else
		return false;
}

void	Channel::addClient(Client &client, clientMode clientMode)
{
	if (this->getClient(&client))
		throw serverExceptions(405);
	if (this->m_password != "")
		throw serverExceptions(475);
	if (client.getChannelsCount() == MAX_CHANNEL_JOINED + 1)
		throw serverExceptions(405);
	if (this->getInviteMode())
		throw serverExceptions(473);
	if (this->getBanned(&client))
		throw serverExceptions(474);
	if (this->m_isChannelSizeLimited && m_vClients.size() >= this->m_sizelimit)
		throw serverExceptions(471);
	s_clientPair res = {&client, clientMode};
	this->m_vClients.push_back(res);
	if (!m_vOP.size())
		addOP(client);
	this->sendJoin(&client);
	this->sendTopic(&client);
	this->sendClientslist(client);
}

void	Channel::addClient(Client &client, const std::string & passwd, clientMode clientMode)
{
	if (this->m_password != passwd)
		throw serverExceptions(475);
	if (this->getClient(&client))
		throw serverExceptions(405);
	if (client.getChannelsCount() == MAX_CHANNEL_JOINED + 1)
		throw serverExceptions(405);
	if (this->m_isChannelSizeLimited && m_vClients.size() == this->m_sizelimit)
		throw serverExceptions(471);
	if (this->getInviteMode())
		throw serverExceptions(473);
	if (this->getBanned(&client))
		throw serverExceptions(474);
	s_clientPair res = {&client, clientMode} ;
	this->m_vClients.push_back(res);
	this->sendJoin(&client);
	this->sendTopic(&client);
	this->sendClientslist(client);
}

void	Channel::removeClient(const Client & client)
{
	for (size_t i = 0; i < m_vClients.size(); i++)
	{
		if (m_vClients[i].client == &client)
		{
			m_vClients.erase(m_vClients.begin() + i);
			if (!m_vClients.size())
				m_serv->deleteChannel(*this);
			return;
		}
	}
}

void Channel::addOP(Client &client) { this->m_vOP.push_back(&client); }

void	Channel::addInvite(Client &client) { this->m_vInvited.push_back(&client); }

void	Channel::sendAllMsg(Server *server, Client *client, const std::string & msg, messageMode mode)
{
	for (size_t i = 0; i < m_vClients.size(); i++)
	{
		if (m_vClients[i].client != client)
		{
			switch (mode)
			{
				case ePrivMsg:
					sendf(server, m_vClients[i].client, PRIVMSG, client->getPrefix().c_str(), msg.c_str());
//					sendf(server, m_vClients[i].client, PRIVMSG, client->getPrefix().c_str(), this->getName().c_str(), msg.c_str());
					break;
				case eNotice:
					sendf(server, m_vClients[i].client, NOTICE, client->getPrefix().c_str(), msg.c_str());
					break;
				case eQuit:
					sendf(server, m_vClients[i].client, QUIT, client->getPrefix().c_str(), msg.c_str());
					break;
				default:
	  				;
			}
		}
	}
}

void	Channel::setTopic(Client *client, const std::string &topic)
{
	if (client && this->getProtectedTopicMode() && this->getClientMode(client) != Operator)
			throw serverExceptions(482);
	if (topic == "\"\"")
		this->m_topic = "";
	else
		this->m_topic = topic;
	m_serv->sendLog("Channel " + this->getName() + "'s topic was set to " + this->m_topic);
	sendAllTopic();
}

const std::string	Channel::getName(void) const { return this->m_name; }

const std::string	Channel::getTopic(void) const {	return this->m_topic; }

void	Channel::setInviteMode(bool status)
{
	this->m_isInviteOnly = status;
	if (status)
		for (size_t i = 0; i < this->m_vClients.size(); i++)
			sendf(this->m_serv, this->m_vClients[i].client, ":%p MODE %c +i", this->getName().c_str());
	else
		for (size_t i = 0; i < this->m_vClients.size(); i++)
			sendf(this->m_serv, this->m_vClients[i].client, ":%p MODE %c -i", this->getName().c_str());
	this->m_serv->sendLog(this->getName() + "'s invite mode was set to " + itoa(status));
}

bool	Channel::getInviteMode() const { return this->m_isInviteOnly; }

void	Channel::setProtectedTopicMode(bool status) 
{
	this->m_isProtectedTopic = status;
	this->m_serv->sendLog(this->getName() + "'s protected topic mode was set to " + itoa(status));

}

bool	Channel::getProtectedTopicMode() const { return this->m_isProtectedTopic; }

void	Channel::setIsSizeLimited(bool status) 
{
	this->m_isChannelSizeLimited = status;
	this->m_serv->sendLog(this->getName() + "'s size limited mode was set to " + itoa(status));
}


void	Channel::setSizeLimit(unsigned int value) 
{
	if (value == 0)
		return  ;
	this->m_sizelimit = value;
	this->m_serv->sendLog(this->getName() + "'s size limit was set to " + itoa(value));
}
		
int Channel::getID() const { return this->m_ID; }

void Channel::setPassword(const std::string &passwd)
{
	m_password = passwd;
	this->m_serv->sendLog(this->getName() + "'s password was set to " + passwd);
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
		if (m_vClients[i].client == client)
			return m_vClients[i].client;
	return NULL;
}

Client *Channel::getClient(const std::string &nickname)
{
	for (size_t i = 0; i < m_vClients.size(); i++)
		if (strCompareNoCase(m_vClients[i].client->getNick(), nickname))
			return m_vClients[i].client;
	return NULL;
}


clientMode	Channel::getClientMode(Client *client)
{
	for (size_t i = 0; i < m_vClients.size(); i++)
		if (m_vClients[i].client == client)
			return m_vClients[i].mode;
	return Default;
}

void Channel::sendAllTopic()
{
	for (std::vector <s_clientPair>::iterator iter = this->m_vClients.begin(); iter != this->m_vClients.end(); iter++)
		sendf(this->m_serv, iter->client, ":%p TOPIC %C :%t", this->getName().c_str(), this->getTopic().c_str());
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

void Channel::sendJoin(Client *client)
{
	for (size_t i = 0; i < this->m_vClients.size(); i++)
		sendf(this->m_serv, this->m_vClients[i].client, ":%m JOIN :%C", client->getPrefix().c_str(), this->getName().c_str());
}

void Channel::sendPart(Client &client, const std::string &message)
{
	for (size_t i = 0; i < this->m_vClients.size(); i++)
		sendf(this->m_serv, this->m_vClients[i].client, ":%m PART %C :%m", client.getPrefix().c_str(), this->getName().c_str(), message.c_str());
}

void Channel::sendKick(Client &source, Client &target, const std::string &message)
{
	for (size_t i = 0; i < this->m_vClients.size(); i++)
		sendf(this->m_serv, this->m_vClients[i].client, ":%P KICK %C %n :%m",source.getPrefix().c_str() ,this->getName().c_str(), target.getNick().c_str(), message.c_str());
}

Server *Channel::getServ() { return m_serv; }

