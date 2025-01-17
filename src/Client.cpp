/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 16:55:26 by eschussl          #+#    #+#             */
/*   Updated: 2025/01/15 13:10:24 by noda             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstddef>
#include <cstdlib>
#include <exception>
#include "Client.hpp"
#include <string>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <vector>
#include "Server.hpp"
#include "utils.hpp"
#include "Channel.hpp"
#include "Numerics.hpp"
#include "serverExceptions.hpp"

/**
 * @brief Sends a quit message and logs the client disconnection.
 * 
 * @param server The server instance.
 * @param str The quit message.
 */
void Client::kill(Server &server, const std::string &str)
{
	this->sendQuitMsg(str);
	server.sendLog(static_cast<std::string>("Client <" + itoa(this->getFD()) + "> Disconnected"));
}

/**
 * @brief Default constructor for the Client class.
 */
Client::Client()
{
	m_isNetCat = false;
	m_authentified = false;
	m_irssiPacket = "";
	m_vChannels.clear();
	m_vOpChannels.clear();
}

/**
 * @brief Destructor for the Client class.
 */
Client::~Client()
{
}

/**
 * @brief Copy constructor for the Client class.
 * 
 * @param copy The Client object to copy.
 */
Client::Client(const Client & copy)
{
	m_isNetCat = copy.m_isNetCat;
	m_fd = copy.m_fd;
	m_authentified = copy.m_authentified;
	m_ipAdd = copy.m_ipAdd;
	m_hostname = copy.m_hostname;
	m_nickname = copy.m_nickname;
	m_username = copy.m_username;
	m_realname = copy.m_realname;
	m_irssiPacket = copy.m_irssiPacket;
	m_vOpChannels = copy.m_vOpChannels;
	m_vChannels = copy.m_vChannels;
}

/**
 * @brief Assignment operator for the Client class.
 * 
 * @param rhs The Client object to assign from.
 * @return Client& The assigned Client object.
 */
Client&	Client::operator=(const Client & rhs)
{
	if (this != & rhs)
	{
		m_isNetCat = rhs.m_isNetCat;
		m_fd = rhs.m_fd;
		m_authentified = rhs.m_authentified;
		m_ipAdd = rhs.m_ipAdd;
		m_hostname = rhs.m_hostname;
		m_nickname = rhs.m_nickname;
		m_username = rhs.m_username;
		m_realname = rhs.m_realname;
		m_irssiPacket = rhs.m_irssiPacket;
		m_vOpChannels = rhs.m_vOpChannels;
		m_vChannels = rhs.m_vChannels;
	}
	return *this;
}

/**
 * @brief Cleans up the client's channels.
 */
void	Client::cleanChannels()
{
	for (size_t i = 0; i < m_vOpChannels.size(); i++)
		m_vOpChannels[i]->getMode()->removeOP(this);
	for (size_t i = 0; i < m_vChannels.size(); i++)
		m_vChannels[i]->removeClient(*this);
}

//getters / setters

/**
 * @brief Gets the client's prefix.
 * 
 * @return std::string The client's prefix.
 */
std::string	Client::getPrefix() const
{
	std::string prefix = this->getNickname() + "!" + this->getUsername() + "@" + this->m_hostname;
	return prefix;
}

/**
 * @brief Checks if the client is using NetCat.
 * 
 * @return true If the client is using NetCat.
 * @return false If the client is not using NetCat.
 */
bool	Client::isNetCat() const { return m_isNetCat; }

/**
 * @brief Sets the NetCat status for the client.
 * 
 * @param value The new NetCat status.
 */
void	Client::setNetCat(bool value) { m_isNetCat = value; }

/**
 * @brief Sets the file descriptor for the client.
 * 
 * @param fd The file descriptor.
 */
void Client::setFD(const int &fd) {	m_fd = fd; }

/**
 * @brief Gets the file descriptor for the client.
 * 
 * @return const int& The file descriptor.
 */
const int& Client::getFD() const { return m_fd; }

/**
 * @brief Sets the IP address for the client.
 * 
 * @param ipadd The IP address.
 */
void Client::setIPadd(const std::string &ipadd) { m_ipAdd = ipadd; }

/**
 * @brief Sets the hostname for the client.
 * 
 * @param addr The sockaddr structure.
 * @param server The server instance.
 */
void	Client::setHostname(struct sockaddr *addr, Server &server)
{
	char	host[NI_MAXHOST];
	int		res = getnameinfo(addr, sizeof(struct sockaddr), host, sizeof(host), NULL, 0, 0);
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

/**
 * @brief Gets the hostname for the client.
 * 
 * @return const std::string& The hostname.
 */
const std::string& Client::getHostName() const { return m_hostname; }

/**
 * @brief Sets the authentication status for the client.
 * 
 * @param is The new authentication status.
 */
void Client::setAuth(const bool &is) { this->m_authentified = is; }

/**
 * @brief Gets the authentication status for the client.
 * 
 * @return bool The authentication status.
 */
bool Client::getAuth() const { return m_authentified; }

/**
 * @brief Sets the username for the client.
 * 
 * @param username The username.
 */
void Client::setUsername(const std::string &username) {	this->m_username = username; }

/**
 * @brief Gets the username for the client.
 * 
 * @return const std::string& The username.
 */
const std::string& Client::getUsername() const { return this->m_username; }

/**
 * @brief Sets the nickname for the client.
 * 
 * @param nick The nickname.
 */
void Client::setNickname(const std::string &nick) {	this->m_nickname = nick; }

/**
 * @brief Gets the nickname for the client.
 * 
 * @return const std::string& The nickname.
 */
const std::string& Client::getNickname() const { return this->m_nickname; }

/**
 * @brief Sets the real name for the client.
 * 
 * @param real The real name.
 */
void Client::setRealname(const std::string &real) { this->m_realname = real; }

/**
 * @brief Gets the real name for the client.
 * 
 * @return const std::string& The real name.
 */
const std::string& Client::getRealname() const { return this->m_realname; }

/**
 * @brief Gets the list of channels the client is in.
 * 
 * @return std::vector<Channel*> The list of channels.
 */
std::vector<Channel*>	Client::getChannel() const { return this->m_vChannels; }

/**
 * @brief Adds a packet to the client's packet buffer.
 * 
 * @param packet The packet to add.
 */
void Client::addPacket(const std::string &packet) {	this->m_irssiPacket += packet; }

/**
 * @brief Gets the client's packet buffer.
 * 
 * @return std::string The packet buffer.
 */
std::string Client::getPacket()
{
	std::string tmp = this->m_irssiPacket;
	this->m_irssiPacket = "";
	return tmp;
}

/**
 * @brief Connects the client to the server.
 * 
 * @param server The server instance.
 */
void Client::connect(Server &server)
{
std::string	motd[] = {
	"",
	"-        Welcome to,",
	"-                   " + server.getHostname(),
	"-",
	"-        * Host.....: " + server.getHostname(),
	"-        * Port.....: " + server.getPort(),
	"-",
	"-    Welcome to our 42 IRC project",
	"-",
	"-           243   38     446297    ",
	"-         53 34908087  313590      ",
	"-      753 735808     33 738     39",
	"-      88 32565       2 198732 9246",
	"-    38979055085      9984504135980          :::::::::: :::::::::::           ::::::::::: :::::::::   :::::::: ",
	"-  057 88495985313   0116007259358          :+:            :+:                   :+:     :+:    :+: :+:    :+:",
	"-   899987   88952257895565940885          +:+            +:+                   +:+     +:+    +:+ +:+",
	"-    5840     588070339903                :#::+::#       +#+                   +#+     +#++:++#:  +#+",
	"-               10136900                 +#+            +#+                   +#+     +#+    +#+ +#+",
	"-             701369805578              #+#            #+#                   #+#     #+#    #+# #+#    #+#",
	"-          71811936088655376           ###            ###    ########## ########### ###    ###  ########",
	"-         10136260   308943335     ",
	"-       36335490       048652233   ",
	"-     708356907          998945557 ",
	"-    39  6490              08994669                      by eschussl and aduvilla",
	"-    050390                 5899959",
	"-  ",
	""};

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
		server.sendLog("Error: Connect: " + static_cast<std::string>(e.what()));
	}
}

/**
 * @brief Adds a channel to the client's list of channels.
 * 
 * @param channel The channel to add.
 */
void Client::addChannel(Channel &channel) { this->m_vChannels.push_back(&channel); }

/**
 * @brief Sends a quit message to all channels the client is in.
 * 
 * @param message The quit message.
 */
void	Client::sendQuitMsg(const std::string & message)
{
	for (size_t i = 0; i < this->m_vChannels.size(); i++)
		this->m_vChannels[i]->sendAllMsg(this->m_vChannels[i]->getServ(), this, message, eQuit);
}

#include <iostream>

/**
 * @brief Removes the client from a channel.
 * 
 * @param channel The channel to leave.
 */
void Client::leaveChannel(Channel &channel)
{
	for (size_t i = 0; i < m_vChannels.size(); i++)
		if (&channel == m_vChannels[i])
			m_vChannels.erase(m_vChannels.begin() + i);
	std::cout << "bug : leave channel " << this->getChannelsCount() << std::endl;
}

/**
 * @brief Adds the client as an operator to a channel.
 * 
 * @param channel The channel to add the client as an operator.
 */
void Client::addOP(Channel &channel) { this->m_vOpChannels.push_back(&channel); }

/**
 * @brief Sends the invite list to the client.
 * 
 * @param server The server instance.
 */
void	Client::sendInviteList(Server * server)
{
	for (size_t i = 0; i < this->m_vChannels.size(); i++)
		server->sendf(this, NULL, this->m_vChannels[i], RPL_INVITELIST);	
	server->sendf(this, NULL, NULL, RPL_ENDOFINVITELIST);
}

/**
 * @brief Removes the client as an operator from a channel.
 * 
 * @param channel The channel to remove the client as an operator.
 */
void Client::leaveOP(Channel &channel)
{
	for (size_t i = 0; i < m_vOpChannels.size(); i++)
		if (&channel == m_vOpChannels[i])
			m_vOpChannels.erase(m_vOpChannels.begin() + i);
}

/**
 * @brief Gets the number of channels the client is in.
 * 
 * @return size_t The number of channels.
 */
size_t Client::getChannelsCount() { return m_vChannels.size(); }
