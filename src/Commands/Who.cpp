/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduvilla <aduvilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 16:15:38 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/14 18:11:25 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Who.hpp"
#include "Channel.hpp"
#include "Parsing.hpp"
#include "Client.hpp"
#include "Numerics.hpp"
#include "Server.hpp"
#include <vector>

/**
 * @brief  Executes the WHO command.
 *
 * This function handles the WHO command, which lists information about users or channels.
 *
 * @param  server : Reference to the server object.
 * @param  parse : Reference to the parsing object containing command arguments.
 * @param  client : Reference to the client object issuing the command.
 */
void	Who::execute(Server &server, const Parsing &parse, Client &client)
{
	if (parse.getArguments().size() < 2)
	{
		server.sendf(&client, &client, NULL, RPL_WHOREPLY, "*", client.getUsername().c_str(), client.getHostName().c_str(), client.getRealname().c_str());
		server.sendf(&client, NULL, NULL, RPL_ENDOFWHO);
	}
	else
	{
		Client *dest = server.getClient(parse.getArguments()[1]);
		Channel *chan = server.getChannel(parse.getArguments()[1]);
		if (dest)
		{
			server.sendf(&client, dest, NULL, RPL_WHOREPLY, dest->getUsername().c_str(), dest->getHostName().c_str(), dest->getRealname().c_str());
			server.sendf(&client, NULL, NULL, RPL_ENDOFWHO, parse.getArguments()[1].c_str());
		}
		else if (chan)
			chan->sendAllMsg(&server, &client, parse.getArguments()[1], eWho);
	}
}
