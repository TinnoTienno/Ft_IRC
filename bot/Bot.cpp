/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduvilla <aduvilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 09:05:12 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/16 09:34:31 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include <arpa/inet.h>
#include <cerrno>
#include <exception>
#include <fstream>
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
#include <map>

bool Bot::m_signal = false;

Bot::Bot(std::string serAdd, std::string channel, std::string password, int port) : m_serAddress(serAdd), m_password(password), m_port(port)
{
	this->m_channel = channel[0] == '#' ? channel : "#" + channel;
//	this->m_channel = "#" + channel;
	this->m_name = "FileHandlerBot";
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
	if ((directory = opendir(FILEDIR)) == NULL)
		throw std::runtime_error("Error: Cannot open directory : " + static_cast<std::string>(FILEDIR));
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

void	Bot::m_createDictionary()
{
	try
	{
		std::ifstream				file(BANDIC);
		if (!file)
			throw std::runtime_error("Cannot find banDic.txt: No ban words");
		if (!this->m_vbanDic.empty())
			this->m_vbanDic.clear();
		std::string	word;
		while (std::getline(file, word)) //	while (file >> word) // read word by word cut with tab space \n ...
			this->m_vbanDic.push_back(word);
		file.close();
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}

int	Bot::init()
{
	try
	{
		m_createList();
		m_createDictionary();
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

void	Bot::m_helloWorld()
{
	speak("PRIVMSG " + this->m_channel + " :Hi everyone i'm an IRC File Transfer Bot\r\n");
	speak("PRIVMSG " + this->m_channel + " :You can ask me the list of transferable files with the command 'PRIVMSG FileHandlerBot !list'\r\n");
	speak("PRIVMSG " + this->m_channel + " :I can transfer you the file you want with the command 'PRIVMSG FileHandlerBot !send [filename]'\r\n");
}

int	Bot::m_run()
{
	char	buffer[513];
//	struct	pollfd pfd;
//	pfd.fd = this->m_serSocket;
//	pfd.events = POLLIN;

	while(this->m_signal == false)
	{
//		if ((poll(&pfd, 1, -1) == -1) || this->m_signal == true)
//			return quit();
//		else if (pfd.revents & POLLIN)
		memset(buffer, 0, sizeof(buffer));
		int	bytes = recv(this->m_serSocket, buffer, sizeof(buffer) - 1, 0);
		if (bytes == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
		{
			usleep(100000);
			continue;
		}
		if (bytes == 0 || bytes < -1)
			return quit();
		buffer[bytes] = '\0';
		std::string message(buffer);
		std::cout << ">> " << message << std::endl;
		if (message.find("376") != std::string::npos)
			speak("JOIN " + this->m_channel + "\r\n" );
		if (message.find("JOIN") != std::string::npos)
			m_helloWorld();
		if (message.find("PRIVMSG") != std::string::npos)
			m_handlePrivMsg(message);
		if (message.find("473") != std::string::npos)
		{
			std::cerr << "Error: FileHandlerBot cannot join channel " + m_channel << std::endl;
			return quit();
		}
	}
	return quit();
}

void	Bot::m_handleRefresh(std::vector<std::string> & tokens)
{
	(void)tokens;
	m_createList();
}

void	Bot::m_handleList(std::vector<std::string> & tokens)
{
	std::string user = tokens[0].substr(0, tokens[0].find("!"));
	if (this->m_vlist.empty())
		speak("PRIVMSG " + user + " Bot's shareDirextory is empty\r\n");
	std::ostringstream messageStream;
	messageStream << "PRIVMSG " << user << " :FileHandlerBot is handling " << this->m_vlist.size() << " files:\r\n"; 
	speak(messageStream.str());
	speak("PRIVMSG " + user + " :Type '!send [filename]' to start transfering file\r\n");
	speak("PRIVMSG " + user + " \r\n");
	for (std::vector<std::string>::iterator it = m_vlist.begin(); it != m_vlist.end(); ++it)
		speak("PRIVMSG " + user + " :" + *it + "\r\n");
	for (size_t i = 0; i < this->m_vlist.size(); i++)
		speak("PRIVMSG " + user + " :" + this->m_vlist[i] + "\r\n");
	speak("PRIVMSG " + user + " :End of shared files list\r\n");
}

void	Bot::m_handleKick(std::vector<std::string> & tokens)
{
	std::string user = tokens[0].substr(0, tokens[0].find("!"));
	for (std::vector<std::string>::iterator ittok = tokens.begin() + 3; ittok != tokens.end(); ++ittok)
		for (std::vector<std::string>::iterator itb = m_vbanDic.begin(); itb != m_vbanDic.end(); ++itb)
			if (toLowerStr(*ittok) == *itb)
			{
				speak("KICK " + m_channel + " " + user + " :Shocking!\r\n");
				return ;
			}
}

void	Bot::m_handlePrivMsg(std::string & message)
{
	std::vector<std::string> tokens = vsplit(message, ' ');
	typedef void (Bot::*CommandFuncion)(std::vector<std::string> &);
	std::map<std::string, CommandFuncion> commandMap;

	commandMap["!send"] = &Bot::m_handleSendFile;
	commandMap["!list"] = &Bot::m_handleList;
	commandMap["!refresh"] = &Bot::m_handleRefresh;

	if (tokens.size() < 4)
		return ;
	std::map<std::string, CommandFuncion>::iterator it = commandMap.find(tokens[3]);
	if (it != commandMap.end())
		(this->*(it->second))(tokens);
	m_handleKick(tokens);
}
