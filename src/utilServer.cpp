/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 16:56:14 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/08 17:06:00 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Parsing.hpp"
#include <map>
#include <string>
#include "Channel.hpp"
#include "utils.hpp"
#include <iomanip>
#include <sstream>
#include "serverExceptions.hpp"
#include <iostream>

bool Server::userErrorCode(Client &client, const Parsing &parse)
{
	client.setUsername(parse.getArguments()[1]);
	client.setRealname(parse.getArguments()[4]);
	return 0;
}

Channel *Server::findChannel(const std::string &channelName)
{
	for (size_t i = 0; i < m_vChannels.size(); i++)
		if (strCompareNoCase(m_vChannels[i].getName(), channelName))
			return &m_vChannels[i];
	return NULL;
}

std::ofstream&	Server::getLogFd() { return m_logFd; }

void	Server::sendLog(const std::string &message)
{
	if (this->getLogFd().is_open())
		this->getLogFd() << getTime() << " :" << message << std::endl;
	std::cerr << message << std::endl;
}
