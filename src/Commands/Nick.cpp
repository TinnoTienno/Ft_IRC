/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 14:21:44 by eschussl          #+#    #+#             */
/*   Updated: 2025/01/15 15:54:38 by noda             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Nick.hpp"
#include "utils.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Parsing.hpp"
#include "serverExceptions.hpp"
#include "Numerics.hpp"
#include <iostream>

/**
 * @brief Executes the NICK command.
 * 
 * @param server The server instance.
 * @param parse The parsed command.
 * @param client The client issuing the command.
 */
void Nick::execute(Server &server, const Parsing &parse, Client &client)
{
	std::string nickTmp = client.getNickname();
	std::string source = client.getPrefix();
	Nick::parseError(server, parse, client);
}

/**
 * @brief Parses and handles errors for the NICK command.
 * 
 * @param server The server instance.
 * @param parse The parsed command.
 * @param client The client issuing the command.
 * @return int 0 if successful, 1 if an error occurred.
 */
int Nick::parseError(Server &server, const Parsing &parse, Client &client)
{
	try
	{
		if (parse.getArguments().size() == 1)
			throw(serverExceptions(431));
		if (!isNickFormatted(parse.getArguments()[1]))
			throw(serverExceptions(432));
		if (server.getClient(parse.getArguments()[1]) && server.getClient(parse.getArguments()[1]) != &client)
			throw(serverExceptions(433));
		server.sendf(&client, NULL, NULL, NICK, parse.getArguments()[1].c_str());
		client.setNickname(parse.getArguments()[1]);
	}
	catch(const serverExceptions& e)
	{
		if (e.getErrorCode() == 431)
			e.sendError(server, &client, NULL);
		else if (e.getErrorCode() == 432)
			e.sendError(server, &client, NULL, parse.getArguments()[1].c_str());
		else if (e.getErrorCode() == 433)
		{
			if (client.getNickname() != "")
				e.sendError(server, &client, NULL, parse.getArguments()[1].c_str());
			else
			{
				std::string newNick = server.getNextGuest();
				server.sendf(&client, NULL, NULL, ":%h 433 %m %m :Nickname is already in use", parse.getArguments()[1].c_str(), parse.getArguments()[1].c_str());
				server.sendf(&client, NULL, NULL, NICK2, parse.getArguments()[1].c_str(), newNick.c_str());
				client.setNickname(newNick);
			}
		}
		else if (e.getErrorCode() == 436)
			e.sendError(server, &client, NULL, parse.getArguments()[1].c_str());
		return 1;
	}
	return 0;
}

/**
 * @brief Checks if a nickname is formatted correctly.
 * 
 * @param nickname The nickname to check.
 * @return true If the nickname is formatted correctly.
 * @return false If the nickname is not formatted correctly.
 */
bool Nick::isNickFormatted(const std::string &nickname)
{
	if (nickname.find_first_of("&@.:!\"") != nickname.npos || !nickname.find("#") || isdigit(nickname[0]))
		return 0;
	return 1;
}



