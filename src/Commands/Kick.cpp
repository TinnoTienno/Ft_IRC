/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 16:57:02 by noda              #+#    #+#             */
/*   Updated: 2025/01/13 17:17:54 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Kick.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "serverExceptions.hpp"
#include "Channel.hpp"
#include "Parsing.hpp"
#include <vector>
#include "utils.hpp"
#include <string>

void Kick::execute(Server &server, const Parsing &parse, Client &client)
{
	Channel *chan;
	try
	{
		if (parse.getArguments().size() < 3)
			throw serverExceptions(461);
		chan = server.findChannel(parse.getArguments()[1]);
		if (!chan)
			throw serverExceptions(403);
		Client *op = chan->getClient(&client);
		if (!op)
			throw serverExceptions(442);
		if (chan->getMode()->isOP(op))
			throw serverExceptions(482);
		std::vector <std::string> names = vsplit(parse.getArguments()[2], ',');
		std::vector <std::string>::iterator iter;
		try
		{
			for (iter = names.begin(); iter != names.end(); iter++)
			{
				Client *target = chan->getClient(*iter);
				if (!target)
					throw serverExceptions(441);
				if (parse.getArguments().size() > 3)
					chan->sendKick(*op, *target, parse.getArguments()[3]);
				else
					chan->sendKick(*op, *target, KICK_DEFAULT_MESSAGE);
				chan->removeClient(*target);
			}
		}
		catch(const serverExceptions& e)
		{
			switch (e.getErrorCode())
			{
			case 441 :
            	e.sendError(server, &client, chan, iter->c_str());
           		break;
			}
		}
	}
	catch(const serverExceptions& e)
	{
		switch (e.getErrorCode())
		{
			case 403 :
				e.sendError(server, &client, NULL, chan->getName().c_str());
				break;
			case 442 :
            	e.sendError(server, &client, NULL, chan->getName().c_str());
            	break;
			case 461 :
            	e.sendError(server, &client, NULL, parse.getCommand().c_str());
            	break;
			case 482 :
            	e.sendError(server, &client, NULL, chan->getName().c_str());
            	break;
		}
	}
}
