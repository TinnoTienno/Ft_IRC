/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 15:38:29 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/06 15:38:04 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP
# include <string>

class Client
{
	private :
		int 		m_fd;
		std::string m_ipAdd;
		bool		m_authentified;
		std::string m_irssiPacket;
		std::string m_nick; 
	public : 
		Client();
		const int& getFD() const;
		void setFD(const int&);
		void setIPadd(const std::string&);
		void setAuth(const bool &);
		const bool& getAuth() const;
		void addPacket(const std::string &);
		std::string getPacket();
		const std::string& getNick() const;
		void setNick(const std::string &);
		void sendMsg(const std::string &) const;
		void kill(const std::string &) const;
		~Client();
}	;

#endif