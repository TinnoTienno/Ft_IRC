/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 16:56:14 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/14 00:17:44 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Parsing.hpp"
#include <string>
#include <cstdarg>
#include "Channel.hpp"
#include "utils.hpp"
#include "serverExceptions.hpp"
#include <iostream>
#include <unistd.h>

bool Server::userErrorCode(Client &client, const Parsing &parse)
{
	client.setUsername(parse.getArguments()[1]);
	client.setRealname(parse.getArguments()[4]);
	return 0;
}

Channel *Server::findChannel(const std::string &channelName)
{
	for (size_t i = 0; i < m_vChannels.size(); i++)
		if (strCompareNoCase(m_vChannels[i]->getName(), channelName))
			return m_vChannels[i];
	return NULL;
}

std::ofstream&	Server::getLogFd() { return m_logFd; }

void	Server::sendLog(const std::string &message)
{
	if (this->getLogFd().is_open())
		this->getLogFd() << getTime() << " :" << message << std::endl;
	std::cerr << message << std::endl;
}

/*	SENDF FORMAT :
	%h -> SERVER HOSTNAME
	%p -> destination Client prefix
	%n -> destination Client nickname
	%P -> source Client prefix
	%N -> source Client nickname
	%C -> Channel name
	%T -> Channel topic

static std::string addVar(Server &server, Client* dest, Client *source, Channel *channel, char identifier, va_list args)
{
	switch (identifier)
	{
		case '%' :
			return "%";
		case 'h' :
			return server.getHostname();
		case 'p' :
			if (dest)
				return dest->getPrefix();
			else
				break;
		case 'n' :
			if (dest)
				return dest->getNickname();
			else
				break;
		case 'P' :
			if (source)
				return source->getPrefix();
			else
				break;
		case 'N' :
			if (source)
				return source->getNickname();
			else
				break;
		case 'C' :
			if (channel)
				return channel->getName();
			else
				break;
		case 'T' :
			if (channel)
				return channel->getTopic();
			else
				break;
		default :
			server.sendLog((std::string)"Bug : char " + identifier);
			const char * i = static_cast<const char*>(va_arg(args, const char *));
			if (!i)
				break;
			return i;
	}
	return "-";
}
*/

static std::string addVar(Server &server, Client* dest, Client *source, Channel *channel, char identifier, va_list args)
{
	switch (identifier)
	{
		case '%' :
			return "%";
		case 'h' :
			return server.getHostname();
		case 'p' :
				return dest ? dest->getPrefix() : "-";
		case 'n' :
				return dest ? dest->getNickname() : "-";
		case 'P' :
				return source ? source->getPrefix() : "-";
		case 'N' :
				return source ? source->getNickname() : "-";
		case 'C' :
				return channel ? channel->getName() : "-";
		case 'T' :
				return channel ? channel->getMode()->getTopic(): "-";
		default :
			server.sendLog((std::string)"Bug : char " + identifier);
			const char * i = static_cast<const char*>(va_arg(args, const char *));
			if (!i)
				return "-";
			return i;
//			server.sendLog((std::string)"Bug : char " + identifier);
//			if (const char * i = static_cast<const char*>(va_arg(args, const char *)))
//			const char * i = static_cast<const char*>(va_arg(args, const char *));
//			return i ? i : "-";
//				return i;
//			return "-";
	}
}

#include <iostream>
void Server::sendf(Client *dest, Client *source, Channel *channel, const std::string str, ...)
{
	va_list args;
	std::string message = "";
	va_start (args, str);
	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] == '%')
			message += addVar(*this, dest, source, channel, str[++i], args);
		else
			message += str[i];
	}
	std::cout << "fd dans sendf : " << dest->getFD() << std::endl;
	this->sendLog(itoa(dest->getFD()) + " << " + message);
	message += "\r\n";
	va_end(args);
	std::cout << "avant ou après la merde ??" << std::endl;
	if (send (dest->getFD(), message.c_str(), message.size(), 0) != static_cast<ssize_t>(message.length()))
		throw std::runtime_error("Failed to send the message: " + message);
}
