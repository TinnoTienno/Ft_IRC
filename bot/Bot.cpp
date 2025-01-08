/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduvilla <aduvilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 09:05:12 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/08 14:28:30 by aduvilla         ###   ########.fr       */
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
#include <sys/socket.h>
#include <vector>

Bot::Bot(std::string serAdd, std::string botname, std::string password, int port) : m_serAddress(serAdd), m_name(botname), m_password(password), m_port(port), m_signal(false) {}

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

int	Bot::init()
{
	try
	{
		m_connectToServer();
		speak("PASS " + this->m_password + "\r\n");
		speak("NICK " + this->m_name + "\r\n");
		speak("USER U-" + this->getUsername() + " 0 * :R-" + this->m_name + "\r\n");
//		m_authenticate();
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
			handlePrivMsg(message);
	}
	return 0;
}

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
	return LocalAddr.sin_addr.s_addr;
}

static std::string	&trimNewLines(std::string & str)
{
	while (!str.empty() && (str[str.size() - 1] == '\r' || str[str.size() - 1] == '\n'))
		str.erase(str.size() - 1, 1);
	return str;
}

int	Bot::handleSendFile(const std::string & user, const std::string & filename)
{
	(void)user;
	(void)filename;
	return 0;
}

void	Bot::handlePrivMsg(const std::string & message)
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
			std::string filename = trimNewLines(tokens[4]);
			handleSendFile(user, filename);
		}
		else
			speak("PRIVMSG " + user + " :Usage: !send [filename]\r\n");
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
