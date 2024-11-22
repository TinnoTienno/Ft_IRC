/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 15:26:49 by eschussl          #+#    #+#             */
/*   Updated: 2024/11/22 17:20:04 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <vector>
#include <sys/socket.h>
#include <poll.h>
#include "Client.hpp"

class Client;
class Server
{
	private :
		int 						m_port;
		int							m_serverSocketFd;
		// static bool 				m_signal;
		std::vector<Client> 		m_clients;
		std::vector<struct pollfd>	m_fds;
	public : 
		Server();
		
		void ServerInit(const std::string&);
		void SerSocket();
		void AcceptNewCLient();
		void ReceiveNewData(const int&);
		
		static void SignalHandler(int);
		
		void CloseFds();
		void ClearClients(int);
}	;

#endif