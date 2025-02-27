/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 17:19:53 by eschussl          #+#    #+#             */
/*   Updated: 2025/01/15 13:26:48 by noda             ###   ########.fr       */
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

/**
 * @brief Executes the JOIN command.
 * 
 * @param server The server instance.
 * @param parse The parsed command.
 * @param client The client issuing the command.
 */
void Join::execute(Server &server, const Parsing &parse, Client &client)
{
	try
	{
		if (parse.getArguments().size() < 2)
			throw (serverExceptions(461));
		execChannels(server, parse, client);
	}
	catch(const serverExceptions& e)
	{
		if (e.getErrorCode() == 461)
			e.sendError(server, &client, NULL, parse.getCommand().c_str());
	}
}

/**
 * @brief Executes the JOIN command for multiple channels.
 * 
 * @param server The server instance.
 * @param parse The parsed command.
 * @param client The client issuing the command.
 */
void Join::execChannels(Server &server, const Parsing &parse, Client &client)
{
	std::vector<std::string> channels = vsplit(parse.getArguments()[1], ',');
	std::vector<std::string> passwords;
	if (parse.getArguments().size() > 2)
		passwords = vsplit(parse.getArguments()[2], ',');
	std::vector<std::string>::iterator iterPasswords = passwords.begin();
	std::vector<std::string>::iterator iterChannels = channels.begin();
	while (iterChannels != channels.end())
	{
		Channel *channel = NULL;
		try
		{
			channel = server.findChannel(*iterChannels);
			if (channel && channel->getClient(&client))
				return ;
			if (iterPasswords != passwords.end() && channel)
				channel->addClient(client, *iterPasswords);
			else if (iterPasswords != passwords.end())
				server.createChannel(*iterChannels, client, *iterPasswords);
			else if (channel)
				channel->addClient(client, "");
			else
				server.createChannel(*iterChannels, client);
		}
		catch(const serverExceptions& e)
		{
			switch (e.getErrorCode())
			{
			case 403 :
				e.sendError(server, &client, NULL, iterChannels->c_str());
				break;
			case 405 :
				e.sendError(server, &client, channel);
				break;
			case 461 :
				e.sendError(server, &client, NULL, parse.getCommand().c_str());
				break;
			case 471 :
				e.sendError(server, &client, channel);
				break;
			case 473 :
				e.sendError(server, &client, channel);
				break;
			case 474 :
				e.sendError(server, &client, channel);
				break;
			case 475 :
				e.sendError(server, &client, channel);
				break;
			case 476 :
				e.sendError(server, &client, NULL, parse.getCommand().c_str());
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
