/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 15:23:07 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/08 16:48:03 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <string>
#include <vector>
#include "Client.hpp"

typedef enum messageMode
{
	ePrivMsg,
	eNotice,
	eQuit
}	messageMode;

typedef enum channelType
{
	Public,
	Secret,
	Private,
	Default
}	channelType;

typedef struct s_channelMode
{
	bool	i;
	std::vector<std::string>	m_vInvitedHostNames;
	bool	t;
	std::string					topic;
	bool	k;
	std::string					password;
	bool	o;
	std::vector <Client *>		m_vOP;
	bool	l;
	size_t						limitedSize;
	std::vector <Client *>		m_vBanned;
}	s_channelMode;

class	Server;
class	Channel
{
	public:
		Channel(Server &server, const std::string &name, Client &client, const std::string &passwd);
		Channel(Server &server, const std::string &name, Client &client);
		~Channel(void);
		
		void	addClient(Client &client, const std::string &passwd);
		void	removeClient(const Client & client);
		
		bool	isJoinable(Client &client);
		
		const	std::string getName() const;
		bool	parseChannelName(const std::string &channelName);
		
		Client *getClient(Client *client);
		Client *getClient(const std::string &nickname);
		Server *getServ();

		//Senders
		std::string clientsList();
		std::string getSymbol();
		void 	sendAllMsg(Server *server, Client *client, const std::string & msg, messageMode mode);
		void	sendAllJoin(Client &source);
		void 	sendTopic(Client &dest);
		void 	sendAllTopic();
		void 	sendPart(Client &client, const std::string &message);
		void 	sendKick(Client &source, Client &target, const std::string &message);
		void 	sendClientslist(Client &dest);
		//sModes
		//Invite
		void	setInviteMode(bool);
		bool	getInviteMode() const;
		void	setInvited(Client &client);
		bool	isInvited(Client &client);
		//Topic
		void	setProtectedTopicMode(bool);
		bool	getProtectedTopicMode() const;
		void 	setTopic(Client &client, const std::string &); // * needed to set at channels creation
		const	std::string getTopic() const;
		//Password
		void	setPasswordMode(bool);
		bool	getPasswordMode();
		void	setPassword(const std::string &passwd);
		bool	isPasswordValid(const std::string str);
		//Operator
		void	addOP(Client &client);
		void	removeOP(Client &client);
		bool	isClientOP(Client &client);
		//Size limit
		void	setSizeLimitMode(bool);
		bool	getSizeLimitMode();
		void	setSizeLimit(unsigned int);
		size_t	getSizeLimit();
		//Bans
		void	setBanned(Client &client);
		bool	isBanned(Client &client);
		
	private:
		s_channelMode				m_sModes;
		std::string					m_name;
		std::vector <Client *>		m_vClients;
		Server 						*m_serv;
		channelType					m_channelType;
};

#endif
