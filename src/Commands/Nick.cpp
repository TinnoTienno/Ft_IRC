/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 14:21:44 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/13 15:22:07 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Nick.hpp"
#include "utils.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Parsing.hpp"
#include <iostream>

int Nick::nickErrorCode(Server *server, Client &client, const Parsing &parse)
{
	if (!parse.getArguments()[1].size())
		return 431;
	if (server->findNick(parse.getArguments()[1]))
		return 433;
	if (!server->isNickFormatted(parse.getArguments()[1]))
		return 432;
	client.setNick(parse.getArguments()[1]);
	return 0;
}

void Nick::execute(Server *server, const Parsing &parse, Client &client)
{
	(void) server;
	(void) parse;
	(void) client;
	std::string nickTmp = client.getNick();
	int errorCode = nickErrorCode(server, client, parse);
	// std::cout << "Nick::execute : " << buffer << std::endl;
	switch (errorCode)
	{
	case 431:
		sendMessage(client.getFD(), server->getHostname(), "431 ", "No nickname given");
		break;
	case 432:
		sendMessage(client.getFD(), server->getHostname(), "432 ", "Erroneus nickname");
		break;
	case 433:
		sendMessage(client.getFD(), server->getHostname(), "433 ", "Nickname already in use");
		break;
	default:
		sendMessage(client.getFD(), server->getHostname(), "001  " + client.getNick(), "Bienvenue sur le serveur IRC");
		sendMessage(client.getFD(), client.getPrefix(server->getHostname()), "NICK " + nickTmp, " " + client.getNick());
	}
}

int Nick::checkformat(Server *server, const std::string &line)
{
	
		return 432; // ERR_NONICKNAMEGIVEN
	if (server->findNick(line))
		return 433;
	return -1;
}
