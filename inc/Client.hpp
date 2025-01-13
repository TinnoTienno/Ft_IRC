/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduvilla <aduvilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 15:38:29 by eschussl          #+#    #+#             */
/*   Updated: 2025/01/13 14:30:14 by aduvilla         ###   ########.fr       */
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
		int 					m_fd;
		bool					m_authentified;
		std::string 			m_ipAdd;
		std::string				m_hostname;
		std::string 			m_nickname;
		std::string 			m_username;
		std::string 			m_realname;
		std::string 			m_irssiPacket;
		std::vector<Channel *> 	m_vOpChannels;
		std::vector<Channel *>	m_vChannels;

	public : 
		Client();
		~Client();

		//getters
		std::string	getPrefix() const;
		const std::string& getUsername() const;
		const int&	getFD() const;
		bool getAuth() const;
		const std::string& getNickname() const;
		const std::string& getRealname() const;
		std::string getPacket();
		std::vector<Channel *>	getChannel() const;
	
	 	// setters
		void	setFD(const int&);
		void	setUsername(const std::string &username);
		void	setIPadd(const std::string&);
		void	setHostname(struct sockaddr *addr, Server &server);
		void	setAuth(const bool &);
		void	setNickname(const std::string &);
		void	setRealname(const std::string &);
		void 	addPacket(const std::string &);

		//messages
		void	sendQuitMsg(const std::string & msg);
		void 	kill(Server & server, const std::string &);
		void	connect(Server &server);
		void	sendInviteList(Server *server);

		//Channels
		void	addChannel(Channel &channel);
		void	leaveChannel(Channel &channel);
		void	addOP(Channel &channel);
		void	leaveOP(Channel &channel);
		size_t	getChannelsCount();
}	;

#endif
