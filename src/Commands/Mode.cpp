/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 19:15:16 by noda              #+#    #+#             */
/*   Updated: 2024/12/22 13:30:53 by noda             ###   ########.fr       */
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

void Mode::execute(Server &server, const Parsing &parse, Client &client)
{
	try
	{
		bool modifier;
		Channel *chan = server.getChannel(parse.getArguments()[1]);
		if (!chan)
			throw serverExceptions(403);
		if (parse.getArguments().size() == 2)
			throw serverExceptions(324);
		if (chan->getClientMode(&client) != Operator)
			throw serverExceptions(482);
		std::cout << parse.getArguments()[2] << std::endl;
		if (parse.getArguments()[2] == "+")
			modifier = true;
		else if (parse.getArguments()[2] == "-")
			modifier = false;
		else
			throw serverExceptions(461);
		if (parse.getArguments()[3].find('i'))
			chan->setInviteMode(modifier);
		else if (parse.getArguments()[3].find('t'))
			chan->setProtectedTopicMode(modifier);
		else if (parse.getArguments()[3].find('l') && !modifier)
			chan->setIsSizeLimited(modifier);
		else if (parse.getArguments()[3].find('l') && parse.getArguments().size() > 3)
		{
			chan->setIsSizeLimited(modifier);
			chan->setSizeLimit(atoi(parse.getArguments()[3]));
		}
		else if (parse.getArguments()[3].find('k') && !modifier)
			chan->setPassword("");
		else if (parse.getArguments()[3].find('k') && parse.getArguments().size() > 3)
			chan->setPassword(parse.getArguments()[3]);
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
