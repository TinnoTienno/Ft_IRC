/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 15:23:07 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/15 13:45:09 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <string>
#include <vector>
#include "Client.hpp"
#include "ChanMode.hpp"

typedef enum messageMode
{
	ePrivMsg,
	eNotice,
	eQuit,
	eWho,
	eJoin,
	ePart,
	eKick,
	eMode,
	eTopic
}	messageMode;

typedef enum channelType
{
	Public,
	Secret,
	Private,
	Default
}	channelType;

class	Server;
class	Channel
{
	public:
		Channel(Server &server, const std::string &name, const std::string &passwd);
		Channel(Server &server, const std::string &name);
		Channel(const Channel & copy);
		~Channel(void);
		Channel&	operator=(const Channel & rhs);

		bool	isEmpty() const;
		bool	isJoinable(Client &client);
		void	addClient(Client &client, const std::string &passwd);
		void	cleanClient();
		
		void	removeClient(const Client & client);
		
		const	std::string getName() const;
		bool	parseChannelName(const std::string &channelName);
		
		Client *getClient(Client *client);
		Client *getClient(const std::string &nickname);
		Server *getServ();
		ChanMode*	getMode();

		//Senders
		std::string clientsList();
		std::string getSymbol();
		void 	sendAllMsg(Server *server, Client *client, const std::string & msg, messageMode mode);
		void 	sendTopic(Client &dest);
		void 	sendKick(Client &source, Client &target, const std::string &message);
		void 	sendClientslist(Client &dest);
		
		void	addOP(Client &client);

		std::vector <Client *>		m_vClients;
	private:
		ChanMode					m_cMode;
		std::string					m_name;
		Server 						*m_serv;
		channelType					m_channelType;
};

#endif
