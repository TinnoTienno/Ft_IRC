/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChanMode.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 18:00:31 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/15 16:21:28 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ChanMode.hpp"
#include "Client.hpp"
#include <algorithm>
#include <cstddef>
#include <string>
#include <vector>
#include "Server.hpp"
#include "Numerics.hpp"
#include "utils.hpp"

/**
 * @brief Constructs a new ChanMode object with default settings.
 */
ChanMode::ChanMode() : m_inviteOnly(false), m_topicProtected(false), m_passwordProtected(false), m_sizeLimited(false), m_clientMax(DEFAULTSIZELIMIT) {}

/**
 * @brief Constructs a new ChanMode object with a password.
 * 
 * @param passwd The password for the channel.
 */
ChanMode::ChanMode(const std::string & passwd) : m_inviteOnly(false), m_topicProtected(false), m_passwordProtected(false), m_password(passwd), m_sizeLimited(false), m_clientMax(DEFAULTSIZELIMIT) {}

/**
 * @brief Destroys the ChanMode object.
 */
ChanMode::~ChanMode() {}

/**
 * @brief Checks if the channel is invite-only.
 * 
 * @return True if the channel is invite-only, otherwise false.
 */
bool	ChanMode::isInviteOnly() const { return m_inviteOnly; }

/**
 * @brief Checks if a client is invited to the channel.
 * 
 * @param client The client to check.
 * @return True if the client is invited, otherwise false.
 */
bool	ChanMode::isInvited(Client & client) { return std::find(m_vInvitedHostNames.begin(), m_vInvitedHostNames.end(), client.getPrefix()) != m_vInvitedHostNames.end(); }

/**
 * @brief Checks if the channel topic is protected.
 * 
 * @return True if the topic is protected, otherwise false.
 */
bool	ChanMode::isTopicProtected() const { return m_topicProtected; }

/**
 * @brief Gets the topic of the channel.
 * 
 * @return A reference to the topic string.
 */
const std::string&	ChanMode::getTopic() const { return m_topic; }

/**
 * @brief Checks if the channel is password protected.
 * 
 * @return True if the channel is password protected, otherwise false.
 */
bool	ChanMode::isPasswordProtected() const { return m_passwordProtected; }

/**
 * @brief Checks if a given password is valid for the channel.
 * 
 * @param pass The password to check.
 * @return True if the password is valid, otherwise false.
 */
bool	ChanMode::isPasswordValid(const std::string & pass) const {	return pass == m_password; }

/**
 * @brief Checks if the channel has a size limit.
 * 
 * @return True if the channel has a size limit, otherwise false.
 */
bool	ChanMode::isSizeLimited() const { return m_sizeLimited; }

/**
 * @brief Gets the maximum number of clients allowed in the channel.
 * 
 * @return The maximum number of clients.
 */
size_t	ChanMode::getLimitSize() const { return m_clientMax; }

/**
 * @brief Checks if a client is an operator in the channel.
 * 
 * @param client The client to check.
 * @return True if the client is an operator, otherwise false.
 */
bool	ChanMode::isOP(Client * client) const { return std::find(m_vOP.begin(), m_vOP.end(), client) != m_vOP.end(); }

/**
 * @brief Checks if a client is banned from the channel.
 * 
 * @param client The client to check.
 * @return True if the client is banned, otherwise false.
 */
bool	ChanMode::isBanned(Client * client) const { return std::find(m_vBanned.begin(), m_vBanned.end(), client->getPrefix()) != m_vBanned.end(); }

/**
 * @brief Gets the list of operator clients.
 * 
 * @return A vector of pointers to the operator clients.
 */
std::vector<Client*>	ChanMode::getOpClient() const { return m_vOP; }

/**
 * @brief Sets the invite-only mode for the channel.
 * 
 * @param value True to enable invite-only mode, otherwise false.
 */
void	ChanMode::setInviteOnly(bool value) { m_inviteOnly = value; }

/**
 * @brief Adds a client to the invited list.
 * 
 * @param client The client to invite.
 */
void	ChanMode::setInvited(Client * client) { m_vInvitedHostNames.push_back(client->getPrefix()); }

/**
 * @brief Sets the topic protection mode for the channel.
 * 
 * @param value True to enable topic protection, otherwise false.
 */
void	ChanMode::setTopicProtected(bool value) { m_topicProtected = value; }

/**
 * @brief Sets the topic of the channel.
 * 
 * @param topic The new topic.
 */
void	ChanMode::setTopic(const std::string & topic)
{
	if (topic == "\"\"")
		m_topic = "";
	else
		m_topic = topic;
}

/**
 * @brief Sets the password protection mode for the channel.
 * 
 * @param value True to enable password protection, otherwise false.
 */
void	ChanMode::setPasswordProtected(bool value) { m_passwordProtected = value; }

/**
 * @brief Sets the password for the channel.
 * 
 * @param passwd The new password.
 */
void	ChanMode::setPassword(const std::string & passwd) { m_password = passwd; }

/**
 * @brief Adds a client as an operator in the channel.
 * 
 * @param client The client to add as an operator.
 */
void	ChanMode::addOP(Client * client) { m_vOP.push_back(client); }

/**
 * @brief Removes a client from the operator list.
 * 
 * @param client The client to remove.
 */
void	ChanMode::removeOP(Client * client)
{
	std::vector<Client*>::iterator iter = std::remove(m_vOP.begin(), m_vOP.end(), client);
	if (iter != m_vOP.end())
		m_vOP.erase(iter, m_vOP.end());
}

/**
 * @brief Sets the size limit mode for the channel.
 * 
 * @param value True to enable size limit, otherwise false.
 */
void	ChanMode::setSizeLimited(bool value) { m_sizeLimited = value; }

/**
 * @brief Sets the maximum number of clients allowed in the channel.
 * 
 * @param size The maximum number of clients.
 */
void	ChanMode::setLimitSize(unsigned int size) { m_clientMax = size; }

/**
 * @brief Bans a client from the channel.
 * 
 * @param client The client to ban.
 */
void	ChanMode::setBanned(Client * client) { m_vBanned.push_back(client->getPrefix()); }

/**
 * @brief Bans a client from the channel by their prefix.
 * 
 * @param bannedPrefix The prefix of the client to ban.
 */
void	ChanMode::setBanned(const std::string &bannedPrefix) { m_vBanned.push_back(bannedPrefix); }

/**
 * @brief Unbans a client from the channel.
 * 
 * @param client The client to unban.
 */
void	ChanMode::unsetBanned(Client * client)
{
	std::vector<std::string>::iterator iter = std::remove(m_vBanned.begin(), m_vBanned.end(), client->getPrefix());
	if (iter != m_vBanned.end())
		m_vBanned.erase(iter, m_vBanned.end());
}

/**
 * @brief Unbans a client from the channel by their prefix.
 * 
 * @param bannedPrefix The prefix of the client to unban.
 */
void	ChanMode::unsetBanned(const std::string &bannedPrefix)
{
	std::vector<std::string>::iterator iter = std::remove(m_vBanned.begin(), m_vBanned.end(), bannedPrefix);
	if (iter != m_vBanned.end())
		m_vBanned.erase(iter, m_vBanned.end());
}

//senders

/**
 * @brief Sends the ban list to a client.
 * 
 * @param server The server instance.
 * @param channel The channel.
 * @param dest The destination client.
 */
void	ChanMode::sendBanList(Server &server, Channel &channel, Client &dest)
{
	for (std::vector<std::string>::iterator iter = m_vBanned.begin(); iter != m_vBanned.end(); iter++)
		server.sendf(&dest, NULL, &channel, RPL_BANLIST, (*iter).c_str());
	server.sendf(&dest, NULL, &channel, RPL_ENDOFBANLIST);
}

/**
 * @brief Converts the channel mode to a string representation.
 * 
 * @return The string representation of the channel mode.
 */
std::string ChanMode::modeToStr()
{
	std::string res = "+";
	if (this->isInviteOnly())
		res += "i";
	if (this->isTopicProtected())
		res += "t";
	if (this->isPasswordProtected())
		res += "k";
	if (this->isSizeLimited())
		res += "l" + itoa(this->getLimitSize());
	return res;
}
