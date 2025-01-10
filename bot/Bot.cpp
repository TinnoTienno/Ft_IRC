/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduvilla <aduvilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 09:05:12 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/09 23:19:34 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include <arpa/inet.h>
#include <cerrno>
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
#include <fcntl.h>
#include <poll.h>

bool Bot::m_signal = false;

Bot::Bot(std::string serAdd, std::string channel, std::string password, int port) : m_serAddress(serAdd), m_password(password), m_port(port)
{
	this->m_channel = "#" + channel;
	this->m_name = "FileHandlerBot";
	this->m_fileDir = "botShareDirectory";
	this->m_serSocket = -1;
	this->m_serSocket = -1;
}

Bot::~Bot() {}

std::string	Bot::getNick() const { return this->m_name; }

std::string	Bot::getUsername() const { return "U-" + this->m_name; }

std::string	Bot::getRealname() const { return "R-" + this->m_name; }

void	Bot::m_connectToServer()
{
	struct sockaddr_in	socketAdd = {};

	socketAdd.sin_family = AF_INET;
	socketAdd.sin_port = htons(this->m_port);
	socketAdd.sin_addr.s_addr = inet_addr(this->m_serAddress.c_str());

	this->m_serSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(this->m_serSocket == -1)
		throw std::runtime_error("Error: Socket failed");
	if(connect(this->m_serSocket, (struct sockaddr*)&socketAdd, (socklen_t)(sizeof(socketAdd))) != 0)
		throw std::runtime_error("Error: Cannot connect to server");
	if (fcntl(this->m_serSocket, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("Error: Failed to set non-blocking mode");

}

void	Bot::m_createList()
{
	DIR* directory;
	struct dirent* ent;
	if ((directory = opendir(this->m_fileDir.c_str())) == NULL)
		throw std::runtime_error("Error: Cannot open directory : " + this->m_fileDir);
	if (!this->m_vlist.empty())
		this->m_vlist.clear();
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
		speak("USER " + this->getUsername() + " 0 * :R-" + this->m_name + "\r\n");
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
	struct	pollfd pfd;
	pfd.fd = this->m_serSocket;
	pfd.events = POLLIN;

	while(this->m_signal == false)
	{
		if ((poll(&pfd, 1, -1) == -1) || this->m_signal == true)
			return quit();
		else if (pfd.revents & POLLIN)
		memset(buffer, 0, sizeof(buffer));
		int	bytes = recv(this->m_serSocket, buffer, sizeof(buffer), 0);
		if (bytes <= 0)
			return quit();
		buffer[bytes] = '\0';
		std::string message(buffer);
		std::cout << ">> " << message << std::endl;
		if (message.find("376") != std::string::npos)
		{
			speak("JOIN " + this->m_channel + "\r\n" );
			speak("PRIVMSG " + this->m_channel + " :Hi everyone i'm an IRC File Transfer Bot\r\n");
			speak("PRIVMSG " + this->m_channel + " :You can ask me the list of transferable files with the command 'PRIVMSG FileHandlerBot !list'\r\n");
			speak("PRIVMSG " + this->m_channel + " :I can transfer you the file you want with the command 'PRIVMSG FileHandlerBot !send [filename]'\r\n");
		}
		if (message.find("PRIVMSG") != std::string::npos)
			m_handlePrivMsg(message);
	}
	return quit();
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
	else if (tokens.size() > 3 && m_trimNewLines(tokens[3]) == ":!refresh")
	{
		std::string user = tokens[0].substr(1, tokens[0].find("!") - 1);
		m_createList();
	//	else if (tokens.size() > 4 && m_trimNewLines(tokens[2]) == "JOIN" && m_trimNewLines(tokens[3]) == this->m_channel)
}

void	Bot::speak(const std::string & msg)
{
	std::cout << "<< " << msg << std::endl;
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
