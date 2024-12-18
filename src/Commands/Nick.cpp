/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 14:21:44 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/18 16:36:08 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Nick.hpp"
#include "utils.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Parsing.hpp"
#include "serverExceptions.hpp"

void Nick::execute(Server *server, const Parsing &parse, Client &client)
{
	std::string nickTmp = client.getNick();
	std::string source = client.getPrefix();
	Nick::parseError(server, parse, client);
}

int Nick::parseError(Server *server, const Parsing &parse, Client &client)
{
	try
	{
		if (!parse.getArguments()[1].size())
			throw(serverExceptions(431));
		if (server->getClient(parse.getArguments()[1]))
			throw(serverExceptions(433));
		if (!isNickFormatted(parse.getArguments()[1]))
			throw(serverExceptions(432));
		client.setNick(parse.getArguments()[1]);
	}
	catch(const serverExceptions& e)
	{
		if (e.getErrorCode() == 431)
			e.sendError(server, &client);
		if (e.getErrorCode() == 432)
			e.sendError(server, &client, parse.getArguments()[1].c_str());
		if (e.getErrorCode() == 433)
			e.sendError(server, &client, parse.getArguments()[1].c_str());
		if (e.getErrorCode() == 436)
			e.sendError(server, &client, parse.getArguments()[1].c_str());
	}
	return 0;
}

bool Nick::isNickFormatted(const std::string &nickname)
{
	if (nickname.find_first_of("&@.:!") != nickname.npos || !nickname.find("#") || isdigit(nickname[0]))
		return 0;
	return 1;
}



