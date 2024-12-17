/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduvilla <aduvilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 17:19:53 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/17 14:17:25 by aduvilla         ###   ########.fr       */
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
	for (size_t i = 1, j = firstPassword; i < firstPassword; i++)
	{
		try
		{
			Channel *channel = server->getChannel(parse.getArguments()[i]);
			if (channel == NULL && firstPassword != parse.getArguments().size())
				server->createChannel(parse.getArguments()[i], client);
			else if (channel == NULL)
				server->createChannel(parse.getArguments()[i], client, parse.getArguments()[j++]);
			else if (firstPassword != parse.getArguments().size())
				channel->addClient(client);
			else
				channel->addClient(client, parse.getArguments()[j++]);
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
	}
}

