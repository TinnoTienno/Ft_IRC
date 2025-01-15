/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 15:26:49 by eschussl          #+#    #+#             */
/*   Updated: 2025/01/14 19:39:57 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <vector>
#include <sys/socket.h>
#include <poll.h>
#include "Client.hpp"
#include <fstream>

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
#define TARG_MAX				10
#define KICK_DEFAULT_MESSAGE	"You've been kicked for absolutely no reason... SHAME"


class Client;
class Channel;
class Parsing;

class Server
{
	private :
		unsigned short						m_port;
//		int 								m_port;
		int									m_serverSocketFd;
		static bool 						m_signal;
		const std::string					m_pass;
		std::vector<struct pollfd>			m_vFds;
		std::string							m_hostname;
		unsigned int						m_nextChannelID;
		std::ofstream						m_logFd;
		std::vector<Client*> 				m_vClients;
		std::vector<Channel*>				m_vChannels;
		
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
		
		std::ofstream&	getLogFd();
		void			sendLog(const std::string &message);
		
		const std::string	getPort() const;
		const std::string	getNextGuest();
		const std::string	getHostname() const;
		const std::string	getUserNumber() const;
		const std::string	getChannelNumber() const;
		Client 	*getClient(int clientFd);
		Client 	*getClient(const std::string &nickname);
		Channel *getChannel(const std::string &channelName);

		void	createChannel(const std::string &name, Client &client);
		void	createChannel(const std::string &name, Client &client, const std::string &passwd);
		void	checkEmptyChan();
		void 	deleteChannel(Channel &channel);
		Channel *findChannel(const std::string &channelName);

		void	sendf(Client *dest, Client *source, Channel *channel, const std::string str, ...);

}	;

#endif
