/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 17:19:53 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/19 01:11:34 by noda             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Join.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include <iostream>
#include "Parsing.hpp"
#include "Channel.hpp"
#include "utils.hpp"
#include "serverExceptions.hpp"

void Join::execute(Server *server, const Parsing &parse, Client &client)
{
	try
	{
		if (parse.getArguments().size() < 2)
			throw (serverExceptions(461));
		execChannels(server, parse, client);
	}
	catch(const serverExceptions& e)
	{
		std::cout << "catch" << std::endl;
		if (e.getErrorCode() == 461)
			e.sendError(server, &client, parse.getCommand().c_str());
	}
}

void Join::execChannels(Server *server, const Parsing &parse, Client &client)
{
	std::vector<std::string> channels = vsplit(parse.getArguments()[1], ',');
	std::vector<std::string> passwords;
	if (parse.getArguments().size() == 3)
		passwords = vsplit(parse.getArguments()[2], ',');
	std::cout << "size : " << channels.size() << std::endl;
	std::vector<std::string>::iterator iterPasswords = passwords.begin();
	std::vector<std::string>::iterator iterChannels = channels.begin();
	while (iterChannels != channels.end())
	{
		try
		{
			std::cout << "iterchannel " << *iterChannels << std::endl;
			Channel *channel = server->findChannel(*iterChannels);
			if (iterPasswords != passwords.end() && channel)
			{
				channel->addClient(client, *iterPasswords);
			}
			else if (iterPasswords != passwords.end())
			{
				server->createChannel(*iterChannels, client, *iterPasswords);
			}
			else if (channel)
			{
				std::cout << "1" << std::endl;
				channel->addClient(client);
			}
			else
			{
				server->createChannel(*iterChannels, client);
			}
		}
		catch(const serverExceptions& e)
		{
			std::cout << e.getErrorCode();
			switch (e.getErrorCode())
			{
			case 403 :
				e.sendError(server, &client, iterChannels->c_str());
				break;
			case 405 :
				e.sendError(server, &client, iterChannels->c_str());
				break;
			case 461 :
				e.sendError(server, &client, parse.getCommand().c_str());
				break;
			case 471 :
				e.sendError(server, &client, iterChannels->c_str());
				break;
			case 473 :
				e.sendError(server, &client, iterChannels->c_str());
				break;
			case 474 :
				e.sendError(server, &client, iterChannels->c_str());
				break;
			case 475 :
				e.sendError(server, &client, iterChannels->c_str());
				break;
			case 476 :
				e.sendError(server, &client, iterChannels->c_str());
				break;
			default:
				break;
			}
		}
		iterChannels++;
		if (iterPasswords != passwords.end())
			iterPasswords++;
	}
}