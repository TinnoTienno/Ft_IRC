/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 16:57:02 by noda              #+#    #+#             */
/*   Updated: 2024/12/21 17:43:35 by noda             ###   ########.fr       */
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
	try
	{
		if (parse.getArguments().size() < 3)
			throw 461;
		Channel *chan = server.findChannel(parse.getArguments()[1]);
		if (!chan)
			throw 403;
		Client *op = chan->getClient(&client);
		if (!op)
			throw 442;
		if (chan->getClientMode(&client) != Operator)
			throw 482;
		std::vector<std::string> names = vsplit(parse.getArguments()[2], ',');
		std::vector<std::string>::iterator iter;
		try
		{
			for (iter = names.begin(); iter != names.end(); iter++)
			{
				Client *target = chan->getClient(*iter);
				if (!target)
					throw 441;
				if (parse.getArguments().size() > 3)
					sendf(&server, target, ":%p KICK %C %n :%m", chan->getName().c_str(), target->getNick().c_str(), parse.getArguments()[3].c_str());
				else
					sendf(&server, target, ":%p KICK %C %n :%m", chan->getName().c_str(), target->getNick().c_str(), KICK_DEFAULT_MESSAGE);
				chan->removeClient(*target);
			}
		}
		catch(const serverExceptions& e)
		{
			switch (e.getErrorCode())
			{
			case 403 :
				e.sendError(server, &client, chan->getName().c_str());
				break;
			case 441 :
            	e.sendError(server, &client, iter->c_str(), chan->getName().c_str());
           		break;
			case 442 :
            	e.sendError(server, &client, chan->getName().c_str());
            	break;
			case 482 :
            	e.sendError(server, &client, chan->getName().c_str());
            	break;
			}
		}
	}
	catch(const serverExceptions& e)
	{
		switch (e.getErrorCode())
		{
			case 461 :
            	e.sendError(server, &client, parse.getCommand().c_str());
            	break;
		}
	}
}