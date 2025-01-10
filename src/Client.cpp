/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 16:55:26 by eschussl          #+#    #+#             */
/*   Updated: 2025/01/10 14:12:53 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void Client::kill(Server *server, const std::string &str) const
{
	std::string msg = "ERROR :Closing Link: " + this->getNick() + "hostname :" + str + "\r\n";
	server->sendLog(static_cast<std::string>("Client <" + itoa(this->getFD()) + "> Disconnected"));
	if (send(this->getFD(), msg.c_str(), msg.size(), 0) != (ssize_t)msg.length())
//		throw std::runtime_error("Failed to send message: " + msg);
		return ;
}

Client::Client() {
	m_authentified = false;
	m_irssiPacket = "";
}

const int& Client::getFD() const { return m_fd; }

void Client::setFD(const int &fd) {	m_fd = fd; }

void Client::setIPadd(const std::string &ipadd) { m_ipAdd = ipadd; }

void	Client::setHost(struct sockaddr *addr, Server &server)
{
	char	host[NI_MAXHOST];
	int		res = getnameinfo(addr, sizeof(&addr), host, sizeof(host), NULL, 0, 0);
	try
	{
		sendf(&server, this, AUTH_IDENT);
		sendf(&server, this, AUTH_LOOKHOST);
		if (!res)
		{
			this->m_host = static_cast<std::string>(host);
			sendf(&server, this, AUTH_HOSTFOUND);
		}
		else
		{
			this->m_host = this->m_ipAdd;
			sendf(&server, this, AUTH_HOSTNOTFOUND);
		}
	}
	catch (const std::exception &e)
	{
		server.sendLog("Error: set host: " + static_cast<std::string>(e.what()));
	}
}

const bool& Client::getAuth() const { return m_authentified; }

void Client::setAuth(const bool &is) { this->m_authentified = is; }

void Client::addPacket(const std::string &packet) {	this->m_irssiPacket += packet; }

std::string Client::getPacket()
{
	std::string tmp = this->m_irssiPacket;
	this->m_irssiPacket = "";
	return tmp;
}

const std::string& Client::getNick() const { return this->m_nick; }

void Client::setNick(const std::string &nick) {	this->m_nick = nick; }

const std::string& Client::getUser() const { return this->m_user; }

void Client::setUser(const std::string &user) {	this->m_user = user; }

const std::string& Client::getReal() const { return this->m_realname; } 

void Client::setReal(const std::string &real) { this->m_realname = real; }
		
void	Client::sendQuitMsg(Server *server, const std::string & msg)
{
	for (size_t i = 0; i < this->m_vChannels.size(); i++)
		this->m_vChannels[i]->sendAllMsg(server, this, msg, eQuit);
}

Client::~Client()
{
	// close(m_fd);
	// for (size_t i = 0; i < m_vChannels.size(); i++)
	// 	m_vChannels[i]->removeClient(, *this);// We have to fix this dont know how tho
	// std::cout << "client is dead" << std::endl;
}

std::string	Client::getPrefix() const
{
	std::string prefix = this->getNick() + "!" + this->getUser() + "@" + this->m_host;
	return prefix;
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
		sendf(&server, this, RPL_WELCOME);
		sendf(&server, this, RPL_YOURHOST);
		sendf(&server, this, RPL_CREATED, getTime().c_str());
		sendf(&server, this, RPL_MYINFO);
		sendf(&server, this, RPL_ISUPPORT);
		sendf(&server, this, RPL_LUSERCLIENT, server.getUserNumber().c_str());
		sendf(&server, this, RPL_LUSEROP);
		sendf(&server, this, RPL_LUSERCHANNELS, server.getChannelNumber().c_str());
		sendf(&server, this, RPL_MOTDSTART);
		for (size_t i = 0; i < sizeof(motd) / sizeof(motd[0]); i++)
			sendf(&server, this, RPL_MOTD, motd[i].c_str());
		sendf(&server, this, RPL_ENDOFMOTD);
	}
	catch (std::exception &e)
	{
		server.sendLog("Error: Connect: " + static_cast<std::string>(e.what()));
	}
}

void	Client::sendInviteList(Server *server)
{
	for (size_t i = 0; i < this->m_vChannels.size(); i++)
		sendf(server, this, RPL_INVITELIST, this->m_vChannels[i]->getName().c_str());
	sendf(server, this, RPL_ENDOFINVITELIST);
}

void Client::addChannel(Channel &channel)
{
	this->m_vChannels.push_back(&channel);
	channel.getServ()->sendLog("Channel " + channel.getName() + " was added to " + this->getNick() + "'s channels list");
}

void Client::addOP(Channel &channel)
{	
	this->m_OpChannels.push_back(&channel);
	channel.getServ()->sendLog("Channel " + channel.getName() + " was added to " + this->getNick() + "'s channels OP list");
}

size_t Client::getChannelsCount() { return m_vChannels.size(); }
