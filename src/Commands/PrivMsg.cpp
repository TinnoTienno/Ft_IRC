/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivMsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 19:39:53 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/07 15:10:17 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PrivMsg.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "Parsing.hpp"
#include "utils.hpp"
#include "serverExceptions.hpp"

/*
void	PrivMsg::execute(Server &server, const Parsing &parse, Client &client)
{
	if (parse.getArguments().size() < 3)
	{
		if (parse.getArguments().size() < 2 || parse.getArguments()[1].find(":", 0) == 0)
			sendf(&server, &client, ERR_NORECIPIENT, parse.getCommand().c_str());
		else
			sendf(&server, &client, ERR_NOTEXTTOSEND);
	std::vector<std::string>	targets = vsplit(parse.getArguments()[1], ',');
	for (size_t i = 0; i < targets.size(); i++)
	{
		if (targets[i].find('#') == 0)
		{
			Channel	*chan = server.getChannel(targets[i]);
			if (!chan)
				sendf(&server, &client, ERR_NOSUCHNICK, parse.getArguments()[1].c_str());
			else if (!chan->isJoinable(&client))
			else
				chan->sendAllMsg(&server, &client, parse.getArguments()[2], ePrivMsg); // !!!! il faut aussi envoyer le client actuel pour la source du msg
		}
		else
	  	{
			Client *user = server.getClient( targets[i]);
			if (!user)
				sendf(&server, &client, ERR_NOSUCHNICK, parse.getArguments()[1].c_str());
			else
				sendf(&server, user, PRIVMSG, client.getPrefix().c_str(), user->getNick().c_str(), parse.getArguments()[2].c_str());
		}
	}
//	else if (no such channel)
//		sendMessage(client.getFD(), server.getHostname(), "404 " + client.getNick() + parse.getArguments()[1], "Cannot send to channel");
}
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
			try
			{
				if (targets[i].find('#') == 0)
				{
					Channel	*chan = server.getChannel(targets[i]);
					if (!chan)
						throw serverExceptions(401);
					else if (!chan->isJoinable(&client))
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
						sendf(&server, user, PRIVMSG, client.getPrefix().c_str(), parse.getArguments()[2].c_str());
				}
			}
	   		catch (const serverExceptions & e)
			{
				e.sendError(server, &client, targets[i].c_str());
			}
		}
	}
	catch (const serverExceptions & e)
	{
		switch (e.getErrorCode())
		{
			case 411 :
	   			e.sendError(server, &client, parse.getCommand().c_str());
				break;
			case 412 :
	   			e.sendError(server, &client);
		}
	}
}
