/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 15:23:07 by aduvilla          #+#    #+#             */
/*   Updated: 2024/12/12 17:41:35 by eschussl         ###   ########.fr       */
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
		void addClient(int clientKey);
		void addClient(Client &client, const std::string &passwd);
		void addClient(int clientKey, const std::string &passwd);
		
		void removeClient(const Client &client);
		void removeClient(int clientKey);
		
		void addOP(Client &client);
		void addOP(int clientKey);
		
		void removeOP(Client &client);
		void removeOP(int clientKey);
		
		void sendAllMsg(const std::string &, Server *server);
		void setName(const std::string &);
		void setTopic(const std::string &);
		void	setInvite(bool);
		
		const std::string getName() const;
		const std::string getTopic() const;
		bool	getInvite() const;
		
	private:
		bool 		m_isInviteOnly;
		std::string m_topic;
		std::string	m_name;
		std::string	m_password;
		std::vector <int> m_vClientKeys;
		std::vector <int> m_vOPKeys;
};

#endif
