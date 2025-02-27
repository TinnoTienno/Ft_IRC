/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 15:32:50 by noda              #+#    #+#             */
/*   Updated: 2025/01/15 13:47:04 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Topic.hpp"
#include "Server.hpp"
#include "serverExceptions.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "Parsing.hpp"

/**
 * @brief  Executes the TOPIC command.
 *
 * This function handles the TOPIC command, which sets or retrieves the topic of a channel.
 *
 * @param  server : Reference to the server object.
 * @param  parse : Reference to the parsing object containing command arguments.
 * @param  client : Reference to the client object issuing the command.
 */
void Topic::execute(Server &server, const Parsing &parse, Client &client)
{
	Channel *chan;
	try
	{
		if (parse.getArguments().size() < 2)
			throw serverExceptions(461);
		chan = server.getChannel(parse.getArguments()[1]);
		if (!chan)
			throw serverExceptions(403);
		Client *cli = chan->getClient(&client);
		if (!cli)
			throw serverExceptions(442);
		if (parse.getArguments().size() >= 3)
		{
			if (chan->getMode()->isTopicProtected() && !chan->getMode()->isOP(&client))
				throw serverExceptions(482);
			chan->getMode()->setTopic(parse.getArguments()[2]);
			chan->sendAllMsg(&server, &client, "", eTopic);
		}
		else
			chan->sendTopic(client);
	}
	catch(const serverExceptions &e)
	{
		switch (e.getErrorCode())
		{
			case 403 :
				e.sendError(server, &client, chan);
				break;
			case 442 :
				e.sendError(server, &client, chan);
				break ;
			case 461 :
				e.sendError(server, &client, NULL);
				break;
			case 482 :
				e.sendError(server, &client, chan);
				break ;
		}
	}
	
}
