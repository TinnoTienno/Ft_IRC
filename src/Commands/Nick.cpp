/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 14:21:44 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/13 19:31:13 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Nick.hpp"
#include "utils.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Parsing.hpp"

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

	std::string nickTmp = client.getNick();
	std::string source = client.getPrefix(server->getHostname());
	int errorCode = nickErrorCode(server, client, parse);
	// std::cout << "Nick::execute : " << buffer << std::endl;
	switch (errorCode)
	{
	case 431:
		sendMessage(client.getFD(), server->getHostname(), "431 " + client.getNick(), parse.getArguments()[1] + " No nickname given");
		break;
	case 432:
		sendMessage(client.getFD(), server->getHostname(), "432 " + client.getNick(), parse.getArguments()[1] + " Erroneus nickname");
		break;
	case 433:
		sendMessage(client.getFD(), server->getHostname(), "433 " + client.getNick(), parse.getArguments()[1] + " Nickname already in use");
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
