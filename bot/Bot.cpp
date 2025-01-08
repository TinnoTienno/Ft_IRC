/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduvilla <aduvilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 09:05:12 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/08 12:49:15 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include <arpa/inet.h>
#include <exception>
#include <iostream>
#include <netinet/in.h>
#include <stdexcept>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>

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
		std::cout << buffer << std::endl;
	}
	return 0;
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
	close(this->m_serSocket);
	std::cout << "ircbot Disconnected" << std::endl;
	return 1;
}
