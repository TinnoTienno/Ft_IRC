/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 15:26:49 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/06 18:16:24 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <vector>
#include <sys/socket.h>
#include <poll.h>
#include <map>
#include "Client.hpp"
#include <map>

class ACommand;
class Client;

class Server
{

	private :
		int 						m_port;
		int							m_serverSocketFd;
		std::vector<Client> 		m_clients;
		std::vector<struct pollfd>	m_fds;
		static bool 				m_signal;
		const std::string			m_pass;					 

	public :
		typedef void	(Server::*commandHandler)(const std::string&);
		Server(const std::string&);
		~Server();
		
		void ServerInit(const std::string&);
		void SerSocket();
		void AcceptNewClient();
		void ReceiveNewData(const int);
		
		bool checkAuth(int fd, const std::string&);
		bool checkNick(const size_t &, const std::string &);
		static void SignalHandler(int);
		typedef void(*CommandHandler)(const std::string&, Server&);
		std::map<std::string, CommandHandler> m_CommandHandlers;
		
		void CloseFds();
		void ClearClients(int);
		void BuildCommandMap();
		void registerCommand(const std::string & command, commandHandler handler);
		void handleCommand(const std::string & command, const std::string & params);
		void handleNick(const std::string & params);
		void handleJoin(const std::string & params);
	private :
		int 						m_port;
		int							m_serverSocketFd;
		std::vector<Client> 		m_clients;
		std::vector<struct pollfd>	m_fds;
		static bool 				m_signal;
		const std::string			m_pass;
		std::map<std::string, commandHandler> commandHandlers;	
}	;

#endif
