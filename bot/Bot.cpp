/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduvilla <aduvilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 09:05:12 by aduvilla          #+#    #+#             */
/*   Updated: 2024/12/27 13:51:52 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include <arpa/inet.h>
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

int	Bot::init()
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
	std::cout << "avant envoi pass" << std::endl;
//	sleep(1);
//	/*
	if(speak("PASS " + this->m_password + "\r\n"))
		return this->quit();
	std::cout << "apres pass" << std::endl;
	if(speak("NICK " + this->m_name + "\r\n"))
		return this->quit();
	std::cout << "apres nick" << std::endl;
	if(speak("USER U-" + this->m_name + " 0 * :R-" + this->m_name + "\r\n"))
		return this->quit();
	std::cout << "fin d'envoie des infos" << std::endl;
//	*/
	if(this->m_run())
		return this->quit();
	return 0;
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

int	Bot::speak(const std::string & msg)
{
	std::cout << msg << std::endl;
	ssize_t bytes = send(this->m_serSocket, msg.c_str(), msg.size(), 0);
	if (bytes != (ssize_t)msg.length())
	{
		std::cout << "Error: Message not sent" << std::endl;
		return 1;
	}
	return 0;
}

int	Bot::quit()
{
	close(this->m_serSocket);
	std::cout << "ircbot Disconected" << std::endl;
	return 1;
}
