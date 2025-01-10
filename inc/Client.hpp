/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 15:38:29 by eschussl          #+#    #+#             */
/*   Updated: 2025/01/10 15:08:04 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP
# include <string>
# include <vector>

class Server;
class Channel;

class Client
{
	private :
		int 		m_fd;
		std::string m_ipAdd;
		std::string	m_host;
		bool		m_authentified;
		std::string m_nick;
		std::string m_user;
		std::string m_realname;
		std::string m_irssiPacket;
		std::vector<Channel *>	m_vChannels;
		std::vector<Channel *> 	m_OpChannels;
	public : 
		Client();
		void setFD(const int&);
		void setIPadd(const std::string&);
		void setHost(struct sockaddr *addr, Server &server);
		void setAuth(const bool &);
		void setUser(const std::string &user);
		void setNick(const std::string &);
		void setReal(const std::string &);

		const int& getFD() const;
		const bool& getAuth() const;
		const std::string& getUser() const;
		const std::string& getNick() const;
		const std::string& getReal() const;
		std::string	getPrefix() const;
		std::string getPacket();
		
		void 	addPacket(const std::string &);
		void	sendQuitMsg(Server *server, const std::string & msg);
		void 	kill(Server *server, const std::string &) const;
		void	connect(Server &server);

		void	sendInviteList(Server *server);
		void	addChannel(Channel &channel);
		void	addOP(Channel &channel);
		size_t	getChannelsCount();
		~Client();
}	;

#endif
