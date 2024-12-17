/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduvilla <aduvilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 14:21:44 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/17 15:20:41 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Nick.hpp"
#include "utils.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Parsing.hpp"


void Nick::execute(Server *server, const Parsing &parse, Client &client)
{
	std::string nickTmp = client.getNick();
	std::string source = client.getPrefix();
	int errorCode = Nick::errorCode(server, parse, client);
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

int Nick::errorCode(Server *server, const Parsing &parse, Client &client)
{
	if (!parse.getArguments()[1].size())
		return 431;
	if (server->getClient(parse.getArguments()[1]))
		return 433;
	if (!isNickFormatted(parse.getArguments()[1]))
		return 432;
	client.setNick(parse.getArguments()[1]);
	return 0;
}

bool Nick::isNickFormatted(const std::string &nickname)
{
	if (nickname.find_first_of("&@.:!") != nickname.npos || !nickname.find("#") || isdigit(nickname[0]))
		return 0;
	return 1;
}



