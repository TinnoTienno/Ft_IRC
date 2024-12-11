/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 15:26:49 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/11 17:47:27 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <vector>
#include <sys/socket.h>
#include <poll.h>
#include "Client.hpp"
#include <map>

#define RED "\e[1;31m" //-> for red color
#define WHI "\e[0;37m" //-> for white color
#define GRE "\e[1;32m" //-> for green color
#define YEL "\e[1;33m" //-> for yellow color

class Client;
class Channel;

class Server
{

	private :
		int 						m_port;
		int							m_serverSocketFd;
		static bool 				m_signal;
		const std::string			m_pass;
		std::vector<Client> 		m_vClients;
		std::vector<struct pollfd>	m_vFds;
		std::vector<Channel>		m_vChannels;
		std::string					m_hostname;

	public :
		typedef void	(Server::*commandHandler)(const std::string&);
		Server(const std::string &name, const std::string &password);
		~Server();
		
		void ServerInit(const std::string&);
		void SerSocket();
		void AcceptNewClient();
		void ReceiveNewData(const int);
		
		bool checkAuth(Client &client, const std::string&);
		bool checkNick(Client &client, const std::string &);
		static void SignalHandler(int);
		
		void CloseFds();
		void ClearClients(int);
		void parseCommand(const std::string buffer, Client &client);
		const std::string getHostname() const;

		bool checkUser(Client &client, const std::string &buffer);

		bool findNick(const std::string &nickname);
}	;

#endif
