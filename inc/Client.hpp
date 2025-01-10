/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduvilla <aduvilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 15:38:29 by eschussl          #+#    #+#             */
/*   Updated: 2025/01/10 15:40:24 by aduvilla         ###   ########.fr       */
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
		bool		m_authentified;
		std::string 			m_ipAdd;
		std::string				m_hostname;
		std::string 			m_nickname;
		std::string 			m_username;
		std::string 			m_realname;
		std::string 			m_irssiPacket;
		std::vector<Channel *> 	m_vOpChannels;
	public : 
		std::vector<Channel *>	m_vChannels;
		Client();

		//getters / setters
		std::string	getPrefix() const;
		void setFD(const int&);
		const int&	getFD() const;
		void setIPadd(const std::string&);
		void setHostname(struct sockaddr *addr, Server &server);
		void setAuth(const bool &);
		bool getAuth() const;
		void setUsername(const std::string &username);
		const std::string& getUsername() const;
		void setNickname(const std::string &);
		const std::string& getNickname() const;
		void setRealname(const std::string &);
		const std::string& getRealname() const;
		//packets
		void 	addPacket(const std::string &);
		std::string getPacket();
		//messages
		// void 	sendMsg(const std::string &, Server &server) const;
		void	sendQuitMsg(const std::string & msg);
		void 	kill(const std::string &) const;
		void	connect(Server &server);
		void	sendInviteList(Server *server);
		//vectors

		void	addChannel(Channel &channel);
		void	leaveChannel(Channel &channel);
		void	addOP(Channel &channel);
		void	leaveOP(Channel &channel);
		size_t	getChannelsCount();
		~Client();
}	;

#endif
