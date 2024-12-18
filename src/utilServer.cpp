/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
<<<<<<< HEAD
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 16:56:14 by aduvilla          #+#    #+#             */
/*   Updated: 2024/12/17 17:33:36 by eschussl         ###   ########.fr       */
=======
/*   By: aduvilla <aduvilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 16:56:14 by aduvilla          #+#    #+#             */
/*   Updated: 2024/12/17 16:41:51 by aduvilla         ###   ########.fr       */
>>>>>>> 2aef5294e1965bb4b2a8ba74b2a0886033821d3f
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Parsing.hpp"
<<<<<<< HEAD
#include <map>
#include <string>
#include "Channel.hpp"
#include "utils.hpp"

const std::string Server::getNextGuest()
{
	static int i = 0;
	std::ostringstream	oss;
	oss << std::setfill ('0') << std::setw (3) <<  ++i;
	return ((std::string) "Guest" + oss.str());
}
=======
#include "Client.hpp"
>>>>>>> 2aef5294e1965bb4b2a8ba74b2a0886033821d3f

bool Server::userErrorCode(Client &client, const Parsing &parse)
{
	client.setUser(parse.getArguments()[1]);
	client.setReal(parse.getArguments()[4]);
	return 0;
}
<<<<<<< HEAD

const std::string	Server::getPort() const 
{
	return itoa(this->m_port);
}

Channel *Server::findChannel(const std::string &channelName)
{
	for (size_t i = 0; i < m_vChannels.size(); i++)
		if (!strCompareNoCase(m_vChannels[i].getName(), channelName))
			return &m_vChannels[i];
	return NULL;
}
=======
>>>>>>> 2aef5294e1965bb4b2a8ba74b2a0886033821d3f
