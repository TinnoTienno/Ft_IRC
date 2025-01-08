/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 19:15:16 by noda              #+#    #+#             */
/*   Updated: 2025/01/08 17:09:23 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Mode.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Parsing.hpp"
#include "serverExceptions.hpp"
#include "Channel.hpp"
#include <iostream>
#include "utils.hpp"
#include <cstdlib>

void Mode::execute(Server &server, const Parsing &parse, Client &client)
{
	try
	{
		bool modifier;
		Channel *chan = server.getChannel(parse.getArguments()[1]);
		server.sendLog(itoa(parse.getArguments().size()) + " is size");
		if (!chan)
			throw serverExceptions(403);
		if (parse.getArguments().size() == 2)
			throw serverExceptions(324);
		if (!chan->isClientOP(client))
			throw serverExceptions(482);
		if (parse.getArguments()[2][0] == '+')
			modifier = true;
		else if (parse.getArguments()[2][0] == '-')
			modifier = false;
		else
			throw serverExceptions(461);
		if (parse.getArguments()[2].find('i') != parse.getArguments()[2].npos)
			chan->setInviteMode(modifier);
		else if (parse.getArguments()[2].find('t') != parse.getArguments()[2].npos)
			chan->setProtectedTopicMode(modifier);
		else if (parse.getArguments()[2].find('l') != parse.getArguments()[2].npos && !modifier)
		{
			sendf(&server, &client, ":%p MODE %C -l", chan->getName().c_str());
			chan->setSizeLimitMode(modifier);
		}
		else if (parse.getArguments()[2].find('l') != parse.getArguments()[2].npos && parse.getArguments().size() > 3 && modifier && parse.getArguments()[3].find_first_not_of("0123456789") == parse.getArguments()[3].npos)
		{
			sendf(&server, &client, ":%p MODE %C +l %m", chan->getName().c_str(), parse.getArguments()[3].c_str());
			chan->setSizeLimitMode(modifier);
			chan->setSizeLimit(std::atoi(parse.getArguments()[3].c_str()));
		}
		else if (parse.getArguments()[2].find('k') != parse.getArguments()[2].npos)
		{	
			chan->setPasswordMode(modifier);
			if (parse.getArguments().size() > 3)
				chan->setPassword(parse.getArguments()[3]);
			else
				chan->setPassword("");
		}
	}
	catch(const serverExceptions& e)
	{
		switch (e.getErrorCode())
		{
		case 403 :
			/* code */
			break;
		case 324 :
			/* code */
			break;
		case 329 :
			/* code */
			break;
		case 482 :
			/* code */
			break;
		default:
			break;
		}
	}
	
}
