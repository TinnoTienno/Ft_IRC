/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 17:19:53 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/16 15:41:52 by noda             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Join.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include <iostream>
#include "Parsing.hpp"
#include "Channel.hpp"
#include "utils.hpp"

static size_t findPassword(std::vector<std::string> args)
{
	size_t i = 1;
	for (; i < args.size(); i++)
	{
		if (args[i][0] != '#' && args[i][0] != '&')
			return i;
	}
	return i;
}
void Join::execute(Server *server, const Parsing &parse, Client &client)
{
	if (parse.getArguments().size() == 1)
	{
		sendMessage(client.getFD(), server->getHostname(), "461 " + client.getNick(), parse.getArguments()[0] + " Not enough parameters");
		return ;
	}
	size_t firstPassword = findPassword(parse.getArguments());
	for (size_t i = 1; i < firstPassword; i++)
	{
		try
		{
			Channel *channel = server->findChannel(parse.getArguments()[i]);
			if (channel == NULL && firstPassword != parse.getArguments().size())
				server->addChannel(parse.getArguments()[i], client);
			else if (channel == NULL)
				server->addChannel(parse.getArguments()[i], client, parse.getArguments()[firstPassword]);
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
	}
}

