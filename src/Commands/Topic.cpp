/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 15:32:50 by noda              #+#    #+#             */
/*   Updated: 2025/01/08 16:35:51 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Topic.hpp"
#include "Server.hpp"
#include "serverExceptions.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "Parsing.hpp"
#include <iostream>

void Topic::execute(Server &server, const Parsing &parse, Client &client)
{
	try
	{
		if (parse.getArguments().size() < 2)
			throw serverExceptions(461);
		Channel *chan = server.getChannel(parse.getArguments()[1]);
		if (!chan)
			throw serverExceptions(403);
		Client *cli = chan->getClient(&client);
		if (!cli)
			throw serverExceptions(442);
		if (parse.getArguments().size() >= 3)
		{
			std::cout << "|" << parse.getArguments()[2] << "|" << std::endl;
			chan->setTopic(&client, parse.getArguments()[2]);
		}
		else
			chan->sendTopic(client);
	}
	catch(const serverExceptions &e)
	{
		switch (e.getErrorCode())
		{
			case 403 :
				e.sendError(server, &client);
				break;
			case 442 :
				e.sendError(server, &client);
				break ;
			case 461 :
				e.sendError(server, &client);
				break;
			case 482 :
				e.sendError(server, &client);
				break ;
		}
	}
	
}