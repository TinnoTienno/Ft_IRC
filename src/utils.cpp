/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduvilla <aduvilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 17:43:19 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/10 15:46:12 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <stdexcept>
#include <string>
#include <unistd.h>
#include <vector>
#include <ctime>
#include <netdb.h>
#include <vector>
#include <sstream>
#include <stdarg.h>
#include "Server.hpp"
#include "serverExceptions.hpp"
#include "Channel.hpp"
#include <stdio.h>

std::string itoa(int nb)
{
	std::ostringstream oss;
	oss << nb;
	return (std::string) oss.str();
}

const std::string	getTime()
{
	char				buffer[100];	
	std::time_t	now =std::time(0);
	std::tm	*timeStruct = std::gmtime(&now);
	std::strftime(buffer, sizeof(buffer), "%a %b %d %Y at %H:%M:%S GMT", timeStruct);
	return std::string(buffer);
}

std::vector<std::string>	vsplit(const std::string & str, char delimiter)
{
	std::stringstream			strStream(str);
	std::vector<std::string>	result;
	std::string					token;

	while (std::getline(strStream, token, delimiter))
		result.push_back(token);
	return result;
}

/*
void	sendMessage(const int fd, Server &server, const std::string & command, const std::string msg)
{
	std::string message = ":" + server.getHostname() + " " + command + " :" + msg;
	server.sendLog(itoa(fd) + " << " + message);
	message += "\r\n";
	if (send(fd, message.c_str(), message.size(), 0) != (ssize_t)message.length())
		throw std::runtime_error("Failed to send the message: " + msg);
}

std::vector<std::string>	vsplit(const std::string & str, char delimiter)
{
	std::vector<std::string>	result;
	std::string					token;

	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] == delimiter && !token.empty())
		{
			result.push_back(token);
			token.clear();
		}
		else if (str[i] != delimiter)
			token += str[i];  
	}
	if (!token.empty())
		result.push_back(token);
	return result;
}
*/

bool charIsNot(char c, const std::string &plage)
{
	for (std::string::const_iterator iter = plage.begin(); iter != plage.end(); iter++)
		if (c == *iter)
			return false;
	return true;
}

bool strCompareNoCase(const std::string &string1, const std::string &string2)
{
	if (string1.size() != string2.size())
		return false;
	for (size_t i = 0; i < string1.size(); i++)
		if (tolower(string1[i]) != tolower(string2[i]))
			return false;
	return true;
}


