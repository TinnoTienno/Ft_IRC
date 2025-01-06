/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivMsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 19:39:53 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/06 14:02:47 by noda             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PrivMsg.hpp"
#include "Server.hpp"
#include "Err.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "Parsing.hpp"
#include "utils.hpp"
#include "serverExceptions.hpp"

void	PrivMsg::execute(Server &server, const Parsing &parse, Client &client)
{
	if (parse.getArguments()[1].find(":", 0) == 0)
		sendf(&server, &client, ERR_NORECIPIENT, parse.getCommand().c_str());
	else if (parse.getArguments().size() == 2)
		sendf(&server, &client, ERR_NOTEXTTOSEND);
	std::vector<std::string>	targets = vsplit(parse.getArguments()[1], ',');
	for (size_t i = 0; i < targets.size(); i++)
	{
		if (targets[i].find('#') == 0)
		{
			Channel	*chan = server.getChannel(targets[i]);
			if (!chan)
				sendf(&server, &client, ERR_NOSUCHNICK, parse.getArguments()[1].c_str());
			else
//				chan->sendAllMsg(server, &client, PRIVMSG, client.getPrefix().c_str(), chan->getName().c_str(), parse.getArguments()[2].c_str());
				chan->sendAllMsg(server, &client, parse.getArguments()[2], ePrivMsg); // !!!! il faut aussi envoyer le client actuel pour la source du msg
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
