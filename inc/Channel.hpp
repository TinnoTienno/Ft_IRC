/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 15:23:07 by aduvilla          #+#    #+#             */
/*   Updated: 2024/12/16 18:15:44 by noda             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <string>
#include <vector>
#include "Client.hpp"

class	Server;
class	Channel
{
	public:
		Channel(const std::string &name, Client &client, const std::string &passwd);
		Channel(const std::string &name, Client &client);
		Channel(Channel const & src);
		Channel &operator=(Channel const & rhs);
		~Channel(void);
		
		void addClient(Client &client);
		void addClient(Client &client, const std::string &passwd);
		
		void	removeClient(Server *server, const Client & client);
		
		void addOP(Client &client);
		
		void removeOP(Client &client);
		
		void sendAllMsg(const std::string &);
		void sendJoin(const Client &source);
		
		void setName(const std::string &);
		void setTopic(const std::string &);
		void	setInvite(bool);
		
		const std::string getName() const;
		const std::string getTopic() const;
		int getID() const;
		bool	getInvite() const;

		void setPassword(const std::string &passwd);
		
	private:
		bool 					m_isInviteOnly;
		std::string 			m_topic;
		std::string				m_name;
		std::string				m_password;
		std::vector <Client *>	m_vClients;
		std::vector <Client *>	m_vOP;
		int						m_ID;
};

#endif
