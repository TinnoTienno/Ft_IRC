/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 15:38:29 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/13 18:38:17 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP
# include <string>

class Server;

class Client
{
	private :
		int 		m_fd;
		std::string m_ipAdd;
		bool		m_authentified;
		std::string m_nick;
		std::string m_user;
		std::string m_realname;
		std::string m_irssiPacket;
		// std::vector<Channel *>	m_vChannels; Not very useful so far, not implemented
	public : 
		std::string	getPrefix() const;
		Client();
		const int& getFD() const;
		void setFD(const int&);
		void setIPadd(const std::string&);
		std::string getIPadd() const;
		void setAuth(const bool &);
		const bool& getAuth() const;
		const std::string& getUser() const;
		void setUser(const std::string &user);
		const std::string& getNick() const;
		void setNick(const std::string &);
		const std::string& getReal() const;
		void setReal(const std::string &);
		
		void addPacket(const std::string &);
		std::string getPacket();
		void sendMsg(const std::string &, Server *server) const;
		void kill(const std::string &) const;
		void connect(Server *server);
		
		~Client();
}	;

#endif
