/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 15:23:07 by aduvilla          #+#    #+#             */
/*   Updated: 2024/12/21 16:40:53 by noda             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <string>
#include <vector>
#include "Client.hpp"


typedef enum Mode
{
	Founder, 
	Protected, 
	Operator, 
	Halfop, 
	Voice,
	Default
}	Mode;

typedef enum channelMode
{
	Public,
	Secret,
	Private,
	Default1
}	channelMode;

typedef struct s_clientPair
{
	Client *client;
	Mode mode;
}	s_clientPair;

class	Server;
class	Channel
{
	public:
		Channel(Server &server, const std::string &name, Client &client, const std::string &passwd);
		Channel(Server &server, const std::string &name, Client &client);
		~Channel(void);
		
		void addClient(Client &client, Mode clientMode);
		void addClient(Client &client, const std::string &passwd, Mode clientMode);
		
		void removeClient(Server &server, const Client & client);
		
		void addOP(Client &client);
		
		void removeOP(Client &client);
		
		void sendAllMsg(Server &server, Client *client, const std::string &);
		void sendJoin(Client &source);
		void sendPart(Client &client, const std::string &message);
		void sendTopic(Client *client);
		void sendAllTopic();
		
		void 	setName(const std::string &);
		void 	setTopic(Client *client, const std::string &);
		
		const std::string getName() const;
		const std::string getTopic() const;
		int getID() const;
		void	setInviteMode(bool);
		bool	getInviteMode() const;
		void	setProtectedTopicMode(bool);
		bool	getProtectedTopicMode() const;

		void setPassword(const std::string &passwd);
		bool parseChannelName(const std::string &channelName);
		
		Client *getBanned(Client *client);
		Client *getClient(Client *client);
		Mode	getClientMode(Client *client);
		Server *getServ();

		void sendClientslist(Client &dest);
		std::string clientsList();
		
		std::string getSymbol();
		
	private:
		bool 					m_isInviteOnly;
		bool					m_isProtectedTopic;
		std::string 			m_topic;
		std::string				m_name;
		std::string				m_password;
		std::vector <s_clientPair>	m_vClients;
		std::vector <Client *>	m_vOP;
		std::vector <Client *>	m_vBans;
		Server 					*m_serv;
		int						m_ID;
		channelMode				m_channelMode;
};

#endif
