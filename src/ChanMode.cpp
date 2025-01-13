/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChanMode.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduvilla <aduvilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 18:00:31 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/12 13:57:21 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ChanMode.hpp"
#include "Client.hpp"
#include <algorithm>
#include <cstddef>
#include <string>
#include <vector>

ChanMode::ChanMode() : m_inviteOnly(false), m_topicProtected(false), m_passwordProtected(false), m_sizeLimited(false), m_clientMax(DEFAULTSIZELIMIT) {}

ChanMode::ChanMode(const std::string & passwd) : m_inviteOnly(false), m_topicProtected(false), m_passwordProtected(false), m_password(passwd), m_sizeLimited(false), m_clientMax(DEFAULTSIZELIMIT) {}

ChanMode::~ChanMode() {}

bool	ChanMode::isInviteOnly() const { return m_inviteOnly; }

bool	ChanMode::isInvited(Client & client) { return std::find(m_vInvitedHostNames.begin(), m_vInvitedHostNames.end(), client.getPrefix()) != m_vInvitedHostNames.end(); }

bool	ChanMode::isTopicProtected() const { return m_topicProtected; }

const std::string&	ChanMode::getTopic() const { return m_topic; }

bool	ChanMode::isPasswordProtected() const { return m_passwordProtected; }

bool	ChanMode::isPasswordValid(const std::string & pass) const { return pass == m_password; }

bool	ChanMode::isSizeLimited() const { return m_sizeLimited; }

size_t	ChanMode::getLimitSize() const { return m_clientMax; }

bool	ChanMode::isOP(Client * client) const { return std::find(m_vOP.begin(), m_vOP.end(), client) != m_vOP.end(); }

bool	ChanMode::isBanned(Client * client) const { return std::find(m_vBanned.begin(), m_vBanned.end(), client) != m_vBanned.end(); }

std::vector<Client*>	ChanMode::getOpClient() const { return m_vOP; }

void	ChanMode::setInviteOnly(bool value) { m_inviteOnly = value; }

void	ChanMode::setInvited(Client * client) { m_vInvitedHostNames.push_back(client->getPrefix()); }

void	ChanMode::setTopicProtected(bool value) { m_topicProtected = value; }

void	ChanMode::setTopic(const std::string & topic)
{
	if (topic == "\"\"")
		m_topic = "";
	else
		m_topic = topic;
}

void	ChanMode::setPasswordProtected(bool value) { m_passwordProtected = value; }

void	ChanMode::setPassword(const std::string & passwd) { m_password = passwd; }

void	ChanMode::addOP(Client * client) { m_vOP.push_back(client); }

void	ChanMode::removeOP(Client * client)
{
	std::vector<Client*>::iterator itter = std::remove(m_vOP.begin(), m_vOP.end(), client);
	if (itter != m_vOP.end())
		m_vOP.erase(itter, m_vOP.end());
}

void	ChanMode::setSizeLimited(bool value) { m_sizeLimited = value; }

void	ChanMode::setLimitSize(unsigned int size) { m_clientMax = size; }

void	ChanMode::setBanned(Client * client) { m_vBanned.push_back(client); }

void	ChanMode::unsetBanned(Client * client)
{
	std::vector<Client*>::iterator itter = std::remove(m_vBanned.begin(), m_vBanned.end(), client);
	if (itter != m_vBanned.end())
		m_vBanned.erase(itter, m_vBanned.end());
}



