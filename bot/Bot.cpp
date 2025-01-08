/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduvilla <aduvilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 09:05:12 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/08 20:17:37 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include <arpa/inet.h>
#include <exception>
#include <iostream>
#include <netinet/in.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/socket.h>
#include <vector>

Bot::Bot(std::string serAdd, std::string channel, std::string password, int port) : m_serAddress(serAdd), m_channel(channel), m_password(password), m_port(port), m_signal(false)
{
	this->m_name = "FileHandlerBot";
	this->m_fileDir = "botShareDirectory";
}

Bot::~Bot() {}

std::string	Bot::getNick() const { return this->m_name; }

std::string	Bot::getUsername() const { return "U-" + this->m_name; }

std::string	Bot::getRealname() const { return "R-" + this->m_name; }

void	Bot::m_connectToServer()
{
	struct sockaddr_in	socketAdd;

	socketAdd.sin_family = AF_INET;
	socketAdd.sin_port = htons(this->m_port);
	socketAdd.sin_addr.s_addr = inet_addr(this->m_serAddress.c_str());

	this->m_serSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(this->m_serSocket == -1)
		throw std::runtime_error("Error: Socket failed");
	if(connect(this->m_serSocket, (struct sockaddr*)&socketAdd, (socklen_t)(sizeof(socketAdd))) != 0)
		throw std::runtime_error("Error: Cannot connect to server");

}

void	Bot::m_createList()
{
	DIR* directory;
	struct dirent* ent;
	if ((directory = opendir(this->m_fileDir.c_str())) == NULL)
		throw std::runtime_error("Error: Cannot open directory : " + this->m_fileDir);
	while ((ent = readdir(directory)) != NULL)
	{
		std::string fileName(ent->d_name);
		if (fileName != "." && fileName != ".." && ent->d_type == DT_REG) // ignore "." and ".." files and check if regular file
			this->m_vlist.push_back(fileName);
	}
	closedir(directory);
}

int	Bot::init()
{
	try
	{
		m_createList();
		m_connectToServer();
		speak("PASS " + this->m_password + "\r\n");
		speak("NICK " + this->m_name + "\r\n");
		speak("USER U-" + this->getUsername() + " 0 * :R-" + this->m_name + "\r\n");
		return m_run();
	}
	catch (const std::exception & e)
	{
		std::cerr << e.what() << std::endl;
		return quit();
	}
}

int	Bot::m_run()
{
	char	buffer[513];

	while(this->m_signal == false)
	{
		memset(buffer, 0, sizeof(buffer));
		int	bytes = recv(this->m_serSocket, buffer, sizeof(buffer), 0);
		if (bytes <= 0)
			return 1;
		buffer[bytes] = '\0';
		std::string message(buffer);
		std::cout << ">> " << message << std::endl;
		if (message.find("PRIVMSG") != std::string::npos)
			m_handlePrivMsg(message);
	}
	return 0;
}

std::string	&Bot::m_trimNewLines(std::string & str)
{
	while (!str.empty() && (str[str.size() - 1] == '\r' || str[str.size() - 1] == '\n'))
		str.erase(str.size() - 1, 1);
	return str;
}

void	Bot::m_handleList(const std::string & user)
{
	if (this->m_vlist.empty())
	{
		speak("PRIVMSG " + user + " Bot's shareDirextory is empty\r\n");
	}
	std::ostringstream messageStream;
	messageStream << "PRIVMSG " << user << " :FileHandlerBot is handling " << this->m_vlist.size() << " files:\r\n"; 
	speak(messageStream.str());
	speak("PRIVMSG " + user + " \r\n");
	speak("PRIVMSG " + user + " :Type '!send [filename]' to start transfering file\r\n");
	for (size_t i = 0; i < this->m_vlist.size(); i++)
		speak("PRIVMSG " + user + " :" + this->m_vlist[i] + "\r\n");
	speak("PRIVMSG " + user + " :End of shared files list\r\n");
}

void	Bot::m_handlePrivMsg(const std::string & message)
{
	std::istringstream inputStream(message);
	std::string token;
	std::vector<std::string> tokens;

	while (std::getline(inputStream, token, ' '))
		tokens.push_back(token);

	if (tokens.size() > 3 && tokens[3] == ":!send")
	{
		std::string user = tokens[0].substr(1, tokens[0].find("!") - 1);
		if (tokens.size() > 4)
		{
			std::string filename = m_trimNewLines(tokens[4]);
			m_handleSendFile(user, filename);
		}
		else
			speak("PRIVMSG " + user + " :Usage: !send [filename]\r\n");
	}
	else if (tokens.size() > 3 && m_trimNewLines(tokens[3]) == ":!list")
	{
		std::string user = tokens[0].substr(1, tokens[0].find("!") - 1);
		m_handleList(user);
	}
}

void	Bot::speak(const std::string & msg)
{
	std::cout << msg << std::endl;
	if (send(this->m_serSocket, msg.c_str(), msg.size(), 0) != static_cast<ssize_t>(msg.length()))
		throw std::runtime_error("Error: Message not sent");
}

int	Bot::quit()
{
	speak("QUIT\r\n");
	if (this->m_serSocket != -1)
		close(this->m_serSocket);
	std::cout << "ircbot Disconnected" << std::endl;
	return 1;
}
/*
int	Bot::m_handleSendFile(const std::string & user, const std::string & filename)
{
	try
	{
		u_int32_t localIp = m_getLocalIpInt();
		if (localIp == 0)
		{
			speak("PRIVMSG " + user + " :Error: can't get local IP\r\n");
			return 1;
		}
		std::ifstream file;
		file.open(filename.c_str(), std::ios::binary | std::ios::ate); // binary -> binary mode ; ate -> cursor is placed at the end
		if (!file.is_open())
		{
			speak("PRIVMSG " + user + " :Error: Cannot open file " + filename + "\r\n");
			return 1;
		}
		size_t	fileSize = file.tellg(); // get the file size in bytes with the current cursor position
		file.seekg(0, std::ios::beg); // move the cursor back to the begining
		int	serverSock = socket(AF_INET, SOCK_STREAM, 0);
		if (serverSock == -1)
			throw std::runtime_error("Error: Socket creation failed");
		struct sockaddr_in	serverAddr = {};
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_addr.s_addr = INADDR_ANY;
		serverAddr.sin_port = 0; // OS will chose a random free port
//		if (bind(serverSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
		if (bind(serverSock, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr)) == -1)
		{
			close(serverSock);
			throw std::runtime_error("Error: socket bind failed");
		}
		socklen_t	serverAddrLen = sizeof(serverAddr);
//		if (getsockname(serverSock, (sockaddr*)&serverAddr, &serverAddrLen) == -1) // fill the serverAddr with the ip and the port of the socket
		if (getsockname(serverSock, reinterpret_cast<struct sockaddr*>(&serverAddr), &serverAddrLen) == -1) // fill the serverAddr with the ip and the port of the socket
		{
			close(serverSock);
			throw std::runtime_error("Error: Getting socket name failed");
		}
		int	port = ntohs(serverAddr.sin_port);
		if (listen(serverSock, 1) == -1)
		{
			close(serverSock);
			throw std::runtime_error("Error: Listening on socket failed");
		}
		std::ostringstream DccMessage;
		DccMessage << "PRIVMSG " << user << " :\001DCC SEND " << filename << " " << localIp << " " << port << " " << fileSize << "\001\r\n";
		speak(DccMessage.str());
		struct sockaddr_in clientAddr = {};
		socklen_t	clientAddrLen = sizeof(clientAddr);
//		int	clientSock = accept(serverSock, (sockaddr*)&clientAddr, &clientAddrLen);
		int	clientSock = accept(serverSock, reinterpret_cast<struct sockaddr*>(&clientAddr), &clientAddrLen);
		if (clientSock == -1)
		{
			close(serverSock);
			throw std::runtime_error("Error: accepting connection failed");
		}
		char	buffer[4096];
		std::cout << "on est al" << std::endl;
		while (file.read(buffer, sizeof(buffer)).gcount() > 0)
		{
			std::cout << "bytes envoyés = " << file.gcount() << std::endl;
			if (send(clientSock, buffer, file.gcount(), 0) == -1)
			{
				speak("PRIVMSG " + user + " :Error: Sending file failed\r\n");
				close(clientSock);
				close(serverSock);
				throw std::runtime_error("Error: Sending file failed");
			}
		}
		close(clientSock);
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
*/
