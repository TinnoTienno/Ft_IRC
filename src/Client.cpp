/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduvilla <aduvilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 16:55:26 by eschussl          #+#    #+#             */
/*   Updated: 2025/01/10 18:43:17 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstddef>
#include <exception>
#include "Client.hpp"
#include <string>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include "Server.hpp"
#include "utils.hpp"
#include "Channel.hpp"
#include "Rpl.hpp"
#include "serverExceptions.hpp"

void Client::kill(Server &server, const std::string &str)
{
	this->sendQuitMsg(str);
	server.sendLog(static_cast<std::string>("Client <" + itoa(this->getFD()) + "> Disconnected"));
}

Client::Client() {
	m_authentified = false;
	m_irssiPacket = "";
	m_vChannels.clear();
	m_vOpChannels.clear();
}
	
Client::~Client()
{
	for (size_t i = 0; i < m_vOpChannels.size(); i++)
		m_vOpChannels[i]->removeOP(*this);
	for (size_t i = 0; i < m_vChannels.size(); i++)
		m_vChannels[i]->removeClient(*this);
};

//getters / setters
std::string	Client::getPrefix() const
{
	std::string prefix = this->getNickname() + "!" + this->getUsername() + "@" + this->m_hostname;
	return prefix;
}

void Client::setFD(const int &fd) {	m_fd = fd; }

const int& Client::getFD() const { return m_fd; }

void Client::setIPadd(const std::string &ipadd) { m_ipAdd = ipadd; }

void	Client::setHostname(struct sockaddr *addr, Server &server)
{
	char	host[NI_MAXHOST];
	int		res = getnameinfo(addr, sizeof(&addr), host, sizeof(host), NULL, 0, 0);
	try
	{
		server.sendf(this, NULL, NULL, AUTH_IDENT);
		server.sendf(this, NULL, NULL, AUTH_LOOKHOST);
		if (!res)
		{
			this->m_hostname = static_cast<std::string>(host);
			server.sendf(this, NULL, NULL, AUTH_HOSTFOUND);
		}
		else
		{
			this->m_hostname = this->m_ipAdd;
			server.sendf(this, NULL, NULL, AUTH_HOSTNOTFOUND);
		}
	}
	catch (const std::exception &e)
	{
		server.sendLog("Error: set host: " + static_cast<std::string>(e.what()));
	}
}

void Client::setAuth(const bool &is) { this->m_authentified = is; }

bool Client::getAuth() const { return m_authentified; }

void Client::setUsername(const std::string &username) {	this->m_username = username; }

const std::string& Client::getUsername() const { return this->m_username; }

void Client::setNickname(const std::string &nick) {	this->m_nickname = nick; }

const std::string& Client::getNickname() const { return this->m_nickname; }

void Client::setRealname(const std::string &real) { this->m_realname = real; }

const std::string& Client::getRealname() const { return this->m_realname; }

//packets
void Client::addPacket(const std::string &packet) {	this->m_irssiPacket += packet; }

std::string Client::getPacket()
{
	std::string tmp = this->m_irssiPacket;
	this->m_irssiPacket = "";
	return tmp;
}

void Client::connect(Server &server)
{
	std::string	motd[] = {"-        Welcome to,",
							"-",
							"-        " + server.getHostname(),
							"-",
							"-        * Host.....: " + server.getHostname(),
							"-        * Port.....: " + server.getPort(),
							"-",
							"-        Welcome to our 42 irc project",
							"-        by eschussl and aduvilla"};
	try
	{
		server.sendf(this, NULL, NULL, RPL_WELCOME);
		server.sendf(this, NULL, NULL, RPL_YOURHOST);
		server.sendf(this, NULL, NULL, RPL_CREATED, getTime().c_str());
		server.sendf(this, NULL, NULL, RPL_MYINFO);
		server.sendf(this, NULL, NULL, RPL_ISUPPORT);
		server.sendf(this, NULL, NULL, RPL_LUSERCLIENT, server.getUserNumber().c_str());
		server.sendf(this, NULL, NULL, RPL_LUSEROP);
		server.sendf(this, NULL, NULL, RPL_LUSERCHANNELS, server.getChannelNumber().c_str());
		server.sendf(this, NULL, NULL, RPL_MOTDSTART);
		for (size_t i = 0; i < sizeof(motd) / sizeof(motd[0]); i++)
			server.sendf(this, NULL, NULL, RPL_MOTD, motd[i].c_str());
		server.sendf(this, NULL, NULL, RPL_ENDOFMOTD);
	}
	catch (std::exception &e)
	{
		server.sendLog("Error: Connect: " + (std::string) e.what());
	}
}

//vectors
void Client::addChannel(Channel &channel)
{
	this->m_vChannels.push_back(&channel);
	channel.getServ()->sendLog("Channel " + channel.getName() + " was added to " + this->getNickname() + "'s channels list");
}

void	Client::sendQuitMsg(const std::string & message)
{
	for (size_t i = 0; i < this->m_vChannels.size(); i++)
		this->m_vChannels[i]->sendAllMsg(this->m_vChannels[i]->getServ(), this, message, eQuit);
}

void Client::leaveChannel(Channel &channel)
{
	for (size_t i = 0; i < m_vChannels.size(); i++)
		if (&channel == m_vChannels[i])
			m_vChannels.erase(m_vChannels.begin() + i);
}

void Client::addOP(Channel &channel)
{	
	this->m_vOpChannels.push_back(&channel);
	channel.getServ()->sendLog("Channel " + channel.getName() + " was added to " + this->getNickname() + "'s channels OP list");
}

void	Client::sendInviteList(Server * server)
{
	for (size_t i = 0; i < this->m_vChannels.size(); i++)
		server->sendf(this, NULL, this->m_vChannels[i], RPL_INVITELIST);	
	server->sendf(this, NULL, NULL, RPL_ENDOFINVITELIST);
}

void Client::leaveOP(Channel &channel)
{
	for (size_t i = 0; i < m_vOpChannels.size(); i++)
		if (&channel == m_vOpChannels[i])
		{
			channel.getServ()->sendLog("Removing " + this->getNickname() + " from " + channel.getName() + " channel's OP's list");
			m_vOpChannels.erase(m_vOpChannels.begin() + i);
		}
}

size_t Client::getChannelsCount() { return m_vChannels.size(); }
