/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 14:21:44 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/12 18:26:39 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Nick.hpp"
#include "utils.hpp"
#include "Client.hpp"
#include "Server.hpp"

void Nick::execute(Server *server, const std::string buffer, Client &client)
{
	std::string nickTmp = client.getNick();
	std::string source = client.getPrefix(server->getHostname());
	int errorCode = server->nickErrorCode(client, buffer);
	// std::cout << "Nick::execute : " << buffer << std::endl;
	switch (errorCode)
	{
	case 431:
		sendMessage(client.getFD(), server->getHostname(), "431 " + client.getNick(), buffer + " No nickname given");
		break;
	case 432:
		sendMessage(client.getFD(), server->getHostname(), "432 " + client.getNick(), buffer + " Erroneus nickname");
		break;
	case 433:
		sendMessage(client.getFD(), server->getHostname(), "433 " + client.getNick(), buffer + " Nickname already in use");
		break;
	default:
		sendMessage(client.getFD(), source, "NICK", client.getNick());
	}
}

int Nick::checkformat(Server *server, const std::string &line)
{
		return 432; // ERR_NONICKNAMEGIVEN
	if (server->findNick(line))
		return 433;
	return -1;
}
