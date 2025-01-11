/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChanMode.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduvilla <aduvilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 18:00:31 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/11 19:14:47 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ChanMode.hpp"
#include "Client.hpp"
#include <cstddef>
#include <string>
#include <vector>

ChanMode::ChanMode() : m_inviteOnly(false), m_topicProtected(false), m_passwordProtected(false), m_sizeLimited(false), m_limitedSize(DEFAULTSIZELIMIT) {}

ChanMode::~ChanMode() {}

bool	ChanMode::isInviteOnly() const { return m_inviteOnly; }

bool	ChanMode::isInvited(Client & client)
{

}

bool	ChanMode::isTopicProtected() const { return m_topicProtected; }

const std::string&	ChanMode::getTopic() const { return m_topic; }

bool	ChanMode::isPasswordProtected() const { return m_passwordProtected; }

bool	ChanMode::isPasswordValid(const std::string & pass) const
{

}

bool	ChanMode::isSizeLimited() const { return m_sizeLimited; }

size_t	ChanMode::getLimitSize() const { return m_limitedSize; }

bool	ChanMode::isOP(Client * client) const { return std::find(m_m_vOP.begin(), m_m_vOP.end(), client) != m_m_vOP.end(); }
{

}

bool	ChanMode::isBanned(Client * client) const
{

}

void	ChanMode::setInviteOnly(bool value) { m_inviteOnly = value; }

void	ChanMode::setInvited(Client * client)
{

}

void	ChanMode::setTopicProtected(bool value) { m_topicProtected = value; }

void	ChanMode::setTopic(Client & client, const std::string & topic)
{

}

void	ChanMode::setPasswordProtected(bool value) { m_passwordProtected = value; }

void	ChanMode::setPassword(const std::string & passwd)
{

}
void	ChanMode::addOP(Client * client) { m_m_vOP.push_back(client); }

void	ChanMode::removeOP(Client * client)
{

}

void	ChanMode::setSizeLimited(bool value) { m_sizeLimited = value; }

void	ChanMode::setLimitSize(unsigned int size) { m_limitedSize = size; }

void	ChanMode::setBanned(Client * client) { m_m_vBanned.push_back(client); }

void	ChanMode::unsetBanned(Client * client)
{

}



