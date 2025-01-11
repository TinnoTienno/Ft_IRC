/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduvilla <aduvilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 13:05:36 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/10 16:29:43 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Invite.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "Parsing.hpp"
#include "Rpl.hpp"
#include "utils.hpp"
#include "Server.hpp"
#include "serverExceptions.hpp"

void	Invite::execute(Server &server, const Parsing &parse, Client &client)
{
	try
	{
		Client	*guest;
		Channel	*chan;
		if (parse.getArguments().size() == 1)
			client.sendInviteList(&server);
		else if (parse.getArguments().size() < 3)
				throw serverExceptions(461);
		guest = server.getClient(parse.getArguments()[1]);
		if (!guest)
			throw serverExceptions(401);
		chan = server.getChannel(parse.getArguments()[2]);
		if (!chan)
			throw serverExceptions(403);
		if (!chan->getClient(&client)) // client not member of chan
			throw serverExceptions(442);
		if (chan->getInviteMode() && !chan->isClientOP(client)) // chan in invite-only and client is not OP
			throw serverExceptions(482);
		if (chan->getClient(guest)) // guest already in chan
			throw serverExceptions(443);
		server.sendf(&client, guest, chan, RPL_INVITING);
		server.sendf(guest, &client, chan, INVITE);
		chan->setInvited(*guest);
	}
	catch (const serverExceptions & e)
	{
		switch (e.getErrorCode())
		{
			case 401 :
				e.sendError(server, &client, parse.getArguments()[1].c_str());
				break;
			case 403 :
				e.sendError(server, &client, parse.getArguments()[2].c_str());
				break;
			case 442 :
				e.sendError(server, &client, parse.getArguments()[2].c_str());
				break;
			case 443 :
				e.sendError(server, &client, parse.getArguments()[1].c_str(), parse.getArguments()[2].c_str());
				break;
			case 461 :
				e.sendError(server, &client, parse.getCommand().c_str());
				break;
			case 482 :
				e.sendError(server, &client, parse.getArguments()[2].c_str());
		}
	}
}
