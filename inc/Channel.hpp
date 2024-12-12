/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 15:23:07 by aduvilla          #+#    #+#             */
/*   Updated: 2024/12/12 16:40:01 by eschussl         ###   ########.fr       */
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
		Channel(const std::string &name, const std::string &passwd);
		Channel(const std::string &name);
		Channel(Channel const & src);
		Channel &operator=(Channel const & rhs);
		~Channel(void);
		void addClient(Client&);
		void addClient(Client&, const std::string&passwd);
		void removeClient(const Client&);
		void sendAllMsg(const std::string &, Server *server);
		void setName(const std::string &);
		void setTopic(const std::string &);
		const std::string getName() const;
		const std::string getTopic() const;
		void	setInvite(bool);
		bool	getInvite() const;
		
	private:
		bool 		m_isInviteOnly;
		std::string m_topic;
		std::string	m_name;
		std::string	m_password;
		std::vector <Client*> m_vClients;
};

#endif
