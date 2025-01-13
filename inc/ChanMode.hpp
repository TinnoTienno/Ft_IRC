/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChanMode.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduvilla <aduvilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 17:28:50 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/12 13:52:34 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANMODE_HPP
# define CHANMODE_HPP
#include "Client.hpp"
#include <cstddef>
#include <string>
#include <vector>
# define DEFAULTSIZELIMIT 8

class Channel;
class ChanMode
{
	private:
		bool						m_inviteOnly;
		std::vector<std::string>	m_vInvitedHostNames;
		bool						m_topicProtected;
		std::string					m_topic;
		bool						m_passwordProtected;
		std::string					m_password;
		bool						m_sizeLimited;
		size_t						m_clientMax;
		std::vector<Client*>		m_vOP;
		std::vector<Client*>		m_vBanned;
	public:
		ChanMode();
		ChanMode(const std::string & pass);
		~ChanMode();
		
		//Getters
		bool	isInviteOnly() const;
		bool	isInvited(Client & client);
		bool	isTopicProtected() const;
		const std::string&	getTopic() const;
		bool	isPasswordProtected() const;
		bool	isPasswordValid(const std::string & pass) const;
		bool	isSizeLimited() const;
		size_t	getLimitSize() const;
		bool	isOP(Client * client) const;
		bool	isBanned(Client * client) const;
		std::vector<Client*>	getOpClient() const;
		
		//Setters
		void	setInviteOnly(bool value);
		void	setInvited(Client * client);
		void	setTopicProtected(bool value);
		void	setTopic(const std::string & topic);
		void	setPasswordProtected(bool value);
		void	setPassword(const std::string & passwd);
		void	addOP(Client * client);
		void	removeOP(Client * client);
		void	setSizeLimited(bool value);
		void	setLimitSize(unsigned int size);
		void	setBanned(Client * client);
		void	unsetBanned(Client * client);
};

#endif
