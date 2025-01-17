/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivMsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 19:39:53 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/15 11:45:49 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PrivMsg.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "Parsing.hpp"
#include "utils.hpp"
#include "serverExceptions.hpp"
#include "Numerics.hpp"

/**
 * @brief  Executes the PRIVMSG command.
 *
 * This function handles the PRIVMSG command, which sends a private message to a user or channel.
 *
 * @param  server : Reference to the server object.
 * @param  parse : Reference to the parsing object containing command arguments.
 * @param  client : Reference to the client object issuing the command.
 */
void	PrivMsg::execute(Server &server, const Parsing &parse, Client &client)
{
	try
	{
		if (parse.getArguments().size() < 3)
		{
			if (parse.getArguments().size() < 2 || parse.getArguments()[1].find(":", 0) == 0)
				throw serverExceptions(411);
			else
				throw serverExceptions(412);
		}
		std::vector<std::string>	targets = vsplit(parse.getArguments()[1], ',');
		for (size_t i = 0; i < targets.size(); i++)
		{
			Channel	*chan = NULL;
			try
			{
				if (targets[i].find('#') == 0)
				{
					chan = server.getChannel(targets[i]);
					if (!chan)
						throw serverExceptions(401);
					else if (!chan->isJoinable(client))
						throw serverExceptions(404);
					else
						chan->sendAllMsg(&server, &client, parse.getArguments()[2], ePrivMsg);
				}
				else
				{
					Client *user = server.getClient( targets[i]);
					if (!user)
						throw serverExceptions(401);
					else
						server.sendf(user, &client, NULL, PRIVMSG, parse.getArguments()[2].c_str());
				}
			}
	   		catch (const serverExceptions & e)
			{
				switch (e.getErrorCode())
				{
					case 401 :
						e.sendError(server, &client, NULL, targets[i].c_str());
						break;
					case 404 :
						e.sendError(server, &client, chan);
				}
			}
		}
	}
	catch (const serverExceptions & e)
	{
		switch (e.getErrorCode())
		{
			case 411 :
	   			e.sendError(server, &client, NULL, parse.getCommand().c_str());
				break;
			case 412 :
	   			e.sendError(server, &client, NULL);
		}
	}
}
