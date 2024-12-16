/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 16:56:14 by aduvilla          #+#    #+#             */
/*   Updated: 2024/12/16 17:01:38 by noda             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <sstream>
#include <iomanip>
#include "Parsing.hpp"
#include <map>
#include <string>
#include "Channel.hpp"

const std::string Server::getNextGuest()
{
	static int i = 0;
	std::ostringstream	oss;
	oss << std::setfill ('0') << std::setw (3) <<  ++i;
	
	return ((std::string) "Guest" + oss.str());
}

bool Server::userErrorCode(Client &client, const Parsing &parse)
{
	client.setUser(parse.getArguments()[1]);
	client.setReal(parse.getArguments()[4]);
	return 0;
}

const std::string	Server::getPort() const 
{
	std::ostringstream	oss;
	oss << this->m_port;
	return (std::string) oss.str();
}

Channel *Server::findChannel(const std::string &channelName)
{
	if (m_mChannels.find(channelName) == m_mChannels.end())
		return NULL;
	return (&m_mChannels.find(channelName)->second);
}
