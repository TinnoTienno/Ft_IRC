/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fileTransferBot.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduvilla <aduvilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 17:38:15 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/15 22:02:49 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include <exception>
#include <fcntl.h>
#include <fstream>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <vector>

uint32_t	Bot::m_getLocalIpInt() const
{
	int	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == -1)
		throw ("Error: can't get local IP");

	struct sockaddr_in loopback = {};
	loopback.sin_family = AF_INET;
	loopback.sin_addr.s_addr = inet_addr("127.0.0.1");
	loopback.sin_port = htons(50000); // port 0-1023 -> system 1024-49151 -> app 49152-65535 -> free

	if (connect(sock, reinterpret_cast<struct sockaddr*>(&loopback), sizeof(loopback)) == -1) // connect to the socket localy
	{
		close(sock);
		throw ("Error: can't get local IP");
	}
	struct sockaddr_in LocalAddr = {};
	socklen_t LocalAddrLen = sizeof(LocalAddr);
	if (getsockname(sock, reinterpret_cast<struct sockaddr*>(&LocalAddr), &LocalAddrLen) == -1)
	{
		close(sock);
		throw ("Error: can't get local IP");
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
	 if (fcntl(serverSock, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("Error: socket bind failed");
	if (bind(serverSock, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr)) == -1)
		throw std::runtime_error("Error: socket bind failed");
	socklen_t	serverAddrLen = sizeof(serverAddr);
	if (getsockname(serverSock, reinterpret_cast<struct sockaddr*>(&serverAddr), &serverAddrLen) == -1) // fill the serverAddr with the ip and the port of the socket
		throw std::runtime_error("Error: Getting socket name failed");
	if (listen(serverSock, 1) == -1)
		throw std::runtime_error("Error: Listening on socket failed");
}

bool	Bot::isInList(const std::string & filename)
{
	for (size_t i = 0; i < this->m_vlist.size(); i++)
		if (this->m_vlist[i] == filename)
			return true;
	return false;
}

void	Bot::m_acceptAndSend(int & serverSock, int & clientSock, std::ifstream & file)
{
	int			elapsedTime = 0;

	struct sockaddr_in clientAddr = {};
	socklen_t	clientAddrLen = sizeof(clientAddr);
	while (clientSock == -1 && elapsedTime < DCCTIMEOUT)
	{
		clientSock = accept(serverSock, reinterpret_cast<struct sockaddr*>(&clientAddr), &clientAddrLen);
		if (clientSock == -1)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
			{
				usleep(1000000);
				elapsedTime++;
			}
			else
				throw std::runtime_error("Error: accepting connection failed");
		}
	}
	if (clientSock == -1)
		throw std::runtime_error("Error: accepting connection time out");
	m_sendFileData(clientSock, file);
	close(clientSock);
	file.close();
}

void	Bot::m_sendFileData(int & clientSock, std::ifstream & file)
{
	char	buffer[4096];
	while (file.read(buffer, sizeof(buffer)).gcount() > 0)
	{
		if (send(clientSock, buffer, file.gcount(), 0) == -1)
			throw std::runtime_error("Error: Sending file failed");
	}
}

void	Bot::m_handleSendFile(std::vector<std::string> & tokens)
{
	int		serverSock = -1;
	int		clientSock = -1;
	std::ifstream	file;
	std::string user = tokens[0].substr(1, tokens[0].find("!") - 1);
	try
	{
		if (tokens.size() < 5)
			throw std::runtime_error("Usage: PRIVMSG " + m_name + " !send [filename]");
		std::string filename = m_trimNewLines(tokens[4]);
		u_int32_t localIp = m_getLocalIpInt();
		struct sockaddr_in	serverAddr = {};
		m_createSocket(serverAddr, serverSock);
		int	port = ntohs(serverAddr.sin_port);
		if (!isInList(filename))
			throw std::runtime_error(filename + ": is not in shared dirextory");
		std::string fullPath = this->m_fileDir + "/" + filename;
		file.open(fullPath.c_str(), std::ios::binary | std::ios::ate); // binary -> binary mode ; ate -> cursor is placed at the end
		if (!file.is_open())
			throw std::runtime_error("Error: Cannot open file " + filename);
		size_t	fileSize = getFileSize(file);
		std::ostringstream DccMessage;
		DccMessage << "PRIVMSG " << user << " :\001DCC SEND " << filename << " " << localIp << " " << port << " " << fileSize << "\001\r\n";
		speak(DccMessage.str());
		speak("PRIVMSG " + user + " :File Sent: It will be available for 1 minute\r\n");
		speak("PRIVMSG " + user + " :Type /dcc get -n [newFileName] -d [pathToCopyDir] FileHandlerBot [filename]\r\n");
		m_acceptAndSend(serverSock, clientSock, file);
		close(serverSock);
		speak("PRIVMSG " + user + " :Send complete\r\n");
	}
	catch (const std::exception & e)
	{
		file.close();
		if (serverSock != -1)
			close(serverSock);
		if (clientSock != -1)
			close(clientSock);
		speak("PRIVMSG " + user + " :" + std::string(e.what()) + "\r\n");
	}
}
