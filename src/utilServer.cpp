/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 16:56:14 by aduvilla          #+#    #+#             */
/*   Updated: 2024/12/19 01:14:02 by noda             ###   ########.fr       */
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
	client.setUser(parse.getArguments()[1]);
	client.setReal(parse.getArguments()[4]);
	return 0;
}

Channel *Server::findChannel(const std::string &channelName)
{
	for (size_t i = 0; i < m_vChannels.size(); i++)
		if (strCompareNoCase(m_vChannels[i].getName(), channelName))
			return &m_vChannels[i];
	return NULL;
}
