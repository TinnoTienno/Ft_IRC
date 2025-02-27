/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 16:56:14 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/17 14:43:42 by noda             ###   ########.fr       */
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

/**
 * @brief Sets the username and realname of a client based on parsed arguments.
 * 
 * @param client The client whose details are to be set.
 * @param parse The parsed arguments containing the username and realname.
 * @return Always returns false.
 */
bool Server::userErrorCode(Client &client, const Parsing &parse)
{
	if (parse.getArguments().size() < 5)
		return 1;
	client.setUsername(parse.getArguments()[1]);
	client.setRealname(parse.getArguments()[4]);
	return 0;
}

/**
 * @brief Finds a channel by its name.
 * 
 * @param channelName The name of the channel to find.
 * @return A pointer to the Channel if found, otherwise NULL.
 */
Channel *Server::findChannel(const std::string &channelName)
{
	for (size_t i = 0; i < m_vChannels.size(); i++)
		if (strCompareNoCase(m_vChannels[i]->getName(), channelName))
			return m_vChannels[i];
	return NULL;
}

/**
 * @brief Gets the log file descriptor.
 * 
 * @return A reference to the log file descriptor.
 */
std::ofstream&	Server::getLogFd() { return m_logFd; }

/**
 * @brief Logs a message to the log file and standard error.
 * 
 * @param message The message to log.
 */
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
*/

/**
 * @brief Adds a variable to the formatted string based on the identifier.
 * 
 * @param server The server instance.
 * @param dest The destination client.
 * @param source The source client.
 * @param channel The channel.
 * @param identifier The identifier for the variable.
 * @param args The list of additional arguments.
 * @return The string representation of the variable.
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
			const char * i = static_cast<const char*>(va_arg(args, const char *));
			if (!i)
				return "-";
			return i;
	}
}

#include <iostream>

/**
 * @brief Sends a formatted message to a client.
 * 
 * @param dest The destination client.
 * @param source The source client.
 * @param channel The channel.
 * @param str The format string.
 * @param ... Additional arguments for the format string.
 */
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
	this->sendLog(itoa(dest->getFD()) + " << " + message);
	message += "\r\n";
	va_end(args);
	if (send (dest->getFD(), message.c_str(), message.size(), 0) != static_cast<ssize_t>(message.length()))
		throw std::runtime_error("Failed to send the message: " + message);
}

/**
 * @brief Checks for empty channels and deletes them.
 */
void	Server::checkEmptyChan()
{
	for (size_t i = 0; i < m_vChannels.size(); i++)
	{
		if (m_vChannels[i]->isEmpty())
		{
			deleteChannel(*m_vChannels[i]);
			m_vChannels.erase(m_vChannels.begin() + i);
		}
	}
}

