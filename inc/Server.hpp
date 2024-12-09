/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 15:26:49 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/09 18:35:40 by eschussl         ###   ########.fr       */
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
		std::string 				m_name;
		std::string					m_serverHostname;

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
		typedef void(*CommandHandler)(const std::string&, Server&);
		std::map<std::string, CommandHandler> m_CommandHandlers;
		
		void CloseFds();
		void ClearClients(int);
		void parseCommand(const std::string buffer, Client &client);
		void BuildCommandMap();
		void registerCommand(const std::string & command, commandHandler handler);
		void handleCommand(const std::string & command, const std::string & params);
		void handleNick(const std::string & params);
		void handleJoin(const std::string & params);
		const std::string getName() const;
		const std::string getHostname() const;

		bool checkUser(Client &client, const std::string &buffer);

		void sendMsg(Client &client, const std::string &str, const std::string &code) const;

		bool findNick(const std::string &nickname);
}	;



#endif
