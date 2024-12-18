/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 15:26:49 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/18 15:15:26 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <vector>
#include <map>
#include <sys/socket.h>
#include <poll.h>
#include "Client.hpp"

/*#####################*/
/*	   MACRO PRINT	   */
/*#####################*/

#define RED "\e[1;31m" //-> for red color
#define WHI "\e[0;37m" //-> for white color
#define GRE "\e[1;32m" //-> for green color
#define YEL "\e[1;33m" //-> for yellow color

/*#####################*/
/* Rules of the server */
/*#####################*/

#define CHANNEL_NAME_MIN_LENGTH	2
#define CHANNEL_NAME_MAX_LENGTH	50
#define NICKNAME_MIN_LENGTH		2
#define NICKNAME_MAX_LENGTH		15
#define USERNAME_MIN_LENGTH		2
#define USERNAME_MAX_LENGTH		15
#define REALNAME_MIN_LENGTH		2
#define REALNAME_MAX_LENGTH		15
#define MAX_CHANNEL_JOINED		15
#define DEFAULT_CHANNEL_MODES	"+t +n"


class Client;
class Channel;
class Parsing;

class Server
{

	private :
		int 								m_port;
		int									m_serverSocketFd;
		static bool 						m_signal;
		const std::string					m_pass;
		std::vector<struct pollfd>			m_vFds;
		std::map<int, Client> 				m_mClients;
		std::vector<Channel>				m_vChannels;
		std::string							m_hostname;
		unsigned int						m_nextChannelID;

	public :
		Server(const std::string &name, const std::string &password);
		~Server();
		
		static void SignalHandler(int signum);

		void ServerInit(const std::string&);
		void SerSocket();
		
		void AcceptNewClient();
		void ReceiveNewData(Client &client);
		void parseCommand(const std::string buffer, Client &client);
		std::string parseBuffer(Client &client, std::string buffer);
		
		bool checkAuth(Client &client, const std::string&);
		bool userErrorCode(Client &client, const Parsing &parse);
		
		void CloseFds();
		void ClearClient(Client &client);
	
		const std::string	getPort() const;
		const std::string	getNextGuest();
		const std::string	getHostname() const;
		const std::string	getUserNumber() const;
		const std::string	getChannelNumber() const;
		Client 	*getClient(int clientFd);
		Client 	*getClient(const std::string &nickname);
		Channel *getChannel(const std::string &channelName);

		Channel &createChannel(const std::string &name, Client &client);
		Channel &createChannel(const std::string &name, Client &client, const std::string &passwd);
		void 	deleteChannel(Channel &channel);
		Channel *findChannel(const std::string &channelName);
}	;

#endif
