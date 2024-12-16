/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 15:26:49 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/16 22:44:57 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <vector>
#include <map>
#include <sys/socket.h>
#include <poll.h>
#include "Client.hpp"

#define RED "\e[1;31m" //-> for red color
#define WHI "\e[0;37m" //-> for white color
#define GRE "\e[1;32m" //-> for green color
#define YEL "\e[1;33m" //-> for yellow color

class Client;
class Channel;
class Parsing;

class Server
{

	private :
		int 						m_port;
		int							m_serverSocketFd;
		static bool 				m_signal;
		const std::string			m_pass;
		std::map<int, Client> 		m_mClients;
		std::vector<struct pollfd>	m_vFds;
		std::map<int, Channel>		m_mChannels;
		std::string					m_hostname;

	public :
		typedef void	(Server::*commandHandler)(const std::string&);
		Server(const std::string &name, const std::string &password);
		~Server();
		
		void ServerInit(const std::string&);
		void SerSocket();
		void AcceptNewClient();
		void ReceiveNewData(Client &client);
		std::string parseBuffer(Client &client, std::string buffer);
		
		bool checkAuth(Client &client, const std::string&);
		bool userErrorCode(Client &client, const Parsing &parse);
		static void SignalHandler(int);
		
		void CloseFds();
		void ClearClient(Client &client);
		void parseCommand(const std::string buffer, Client &client);

		bool isNickFormatted(const std::string &nickname) const;
		int findNick(const std::string &nickname);
		int	getNickFd(const int mapIndex) const;
		int findChannel(const std::string &chanName);
		Channel	getChan(const int mapIndex) const;
	
		const std::string	getPort() const;
		const std::string	getNextGuest();
		const std::string	getHostname() const;
		const std::string	getUserNumber() const;
		const std::string	getChannelNumber() const;
		Client &getClient(int clientKey);
}	;

#endif
