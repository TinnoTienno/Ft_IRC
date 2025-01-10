/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fileTransferBot.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduvilla <aduvilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 17:38:15 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/09 14:00:17 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include <fstream>
#include <cstddef>
#include <string>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <ios>

uint32_t	Bot::m_getLocalIpInt() const
{
	int	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == -1)
		return 0;

	struct sockaddr_in loopback = {};
	loopback.sin_family = AF_INET;
	loopback.sin_addr.s_addr = inet_addr("127.0.0.1");
	loopback.sin_port = htons(50000); // port 0-1023 -> system 1024-49151 -> app 49152-65535 -> free

	if (connect(sock, reinterpret_cast<struct sockaddr*>(&loopback), sizeof(loopback)) == -1) // connect to the socket localy
	{
		close(sock);
		return 0;
	}
	struct sockaddr_in LocalAddr = {};
	socklen_t LocalAddrLen = sizeof(LocalAddr);
	if (getsockname(sock, reinterpret_cast<struct sockaddr*>(&LocalAddr), &LocalAddrLen) == -1)
	{
		close(sock);
		return 0;
	}
	close(sock);
	return ntohl(LocalAddr.sin_addr.s_addr);
}

void	Bot::m_createSocket(struct sockaddr_in & serverAddr, int & serverSock)
{
	serverSock = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSock == -1)
		throw std::runtime_error("Error: Socket creation failed");
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = 0; // OS will chose a random free port
	if (bind(serverSock, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr)) == -1)
	{
		close(serverSock);
		throw std::runtime_error("Error: socket bind failed");
	}
	socklen_t	serverAddrLen = sizeof(serverAddr);
	if (getsockname(serverSock, reinterpret_cast<struct sockaddr*>(&serverAddr), &serverAddrLen) == -1) // fill the serverAddr with the ip and the port of the socket
	{
		close(serverSock);
		throw std::runtime_error("Error: Getting socket name failed");
	}
	if (listen(serverSock, 1) == -1)
	{
		close(serverSock);
		throw std::runtime_error("Error: Listening on socket failed");
	}
}

void	Bot::m_acceptAndSend(int serverSock, std::ifstream & file)
{
	struct sockaddr_in clientAddr = {};
	socklen_t	clientAddrLen = sizeof(clientAddr);
	int	clientSock = accept(serverSock, reinterpret_cast<struct sockaddr*>(&clientAddr), &clientAddrLen);
	if (clientSock == -1)
	{
		close(serverSock);
		throw std::runtime_error("Error: accepting connection failed");
	}
	char	buffer[4096];
	while (file.read(buffer, sizeof(buffer)).gcount() > 0)
	{
		if (send(clientSock, buffer, file.gcount(), 0) == -1)
		{
			close(clientSock);
//			close(serverSock);
			throw std::runtime_error("Error: Sending file failed");
		}
	}
	close(clientSock);
}
bool	Bot::isInList(const std::string & user, const std::string & filename)
{
	for (size_t i = 0; i < this->m_vlist.size(); i++)
		if (this->m_vlist[i] == filename)
			return true;
	speak("PRIVMSG " + user + " :" + filename + " is not in shared dirextory\r\n");
	return false;
}

int	Bot::m_handleSendFile(const std::string & user, const std::string & filename)
{
	if (!isInList(user, filename))
		return 1;
	try
	{
		u_int32_t localIp = m_getLocalIpInt();
		if (localIp == 0)
		{
			speak("PRIVMSG " + user + " :Error: can't get local IP\r\n");
			return 1;
		}
		std::string fullPath = this->m_fileDir + "/" + filename;
		std::ifstream file;
		file.open(fullPath.c_str(), std::ios::binary | std::ios::ate); // binary -> binary mode ; ate -> cursor is placed at the end
		if (!file.is_open())
		{
			speak("PRIVMSG " + user + " :Error: Cannot open file " + filename + "\r\n");
			return 1;
		}
		size_t	fileSize = file.tellg(); // get the file size in bytes with the current cursor position
		file.seekg(0, std::ios::beg); // move the cursor back to the begining

		struct sockaddr_in	serverAddr = {};
		int					serverSock = 0;
		m_createSocket(serverAddr, serverSock);
		int	port = ntohs(serverAddr.sin_port);

		std::ostringstream DccMessage;
		DccMessage << "PRIVMSG " << user << " :\001DCC SEND " << filename << " " << localIp << " " << port << " " << fileSize << "\001\r\n";
		speak(DccMessage.str());
		m_acceptAndSend(serverSock, file);
		close(serverSock);
		speak("PRIVMSG " + user + " :Send complete\r\n");
		return 0;
	}
	catch (const std::exception & e)
	{
		speak("PRIVMSG " + user + " :" + std::string(e.what()) + "\r\n");
		return 1;
	}
}
