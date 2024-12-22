/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 17:43:19 by aduvilla          #+#    #+#             */
/*   Updated: 2024/12/22 13:30:12 by noda             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
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

void	sendMessage(const int fd, const std::string & source, const std::string & command, const std::string msg)
{
	std::string message = ":" + source + " " + command + " :" + msg + "\r\n";
	send(fd, message.c_str(), message.size(), 0);
	std::cout << " " << fd << " << " << message << std::endl;
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

std::string itoa(int nb)
{
	std::ostringstream oss;
	oss << nb;
	return (std::string) oss.str();
}

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

static std::string addVar(Server *server, Client* client, char identifier, va_list args)
{
	if (identifier == '%')
		return "%";
	else if (identifier == 'h' && server)
		return server->getHostname();
	else if (identifier == 'p' && client)
		return (client->getPrefix());
	else if (identifier == 'n' && client)
		return client->getNick();
	else if (!charIsNot(identifier, "CcuHmsDPlt"))
	{
		const char * i = (const char*) va_arg(args, const char *);
		return i;
	}
	return "";
}

void sendf(Server *server, Client *dest, const std::string str, ...)
{
	va_list args;
	std::string message = "";
	if (!server || !dest)
		return ;
	va_start (args, str);
	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] == '%')
			message += addVar(server, dest, str[++i], args);
		else
			message += str[i];
	}
	message += "\r\n\0";
	va_end(args);
	send (dest->getFD(), message.c_str(), message.size(), 0);
	std::cout << dest->getFD() << " << " << message << std::endl;
}

std::string getMode(clientMode mode)
{
	switch (mode)
	{
	case Founder :
		return "~";
	case Protected :
		return "&";
	case Operator :
		return "@";
	case Halfop :
		return "%";
	case Voice :
		return "+";
	default:
		return "";
	}
}

int atoi(const std::string &str)
{
	std::stringstream val;
	val << str;
	int res = 0;
	val >> res;
	return res;
}