/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 15:59:44 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/16 18:55:43 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Ping.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "utils.hpp"
#include "Parsing.hpp"

void Ping::execute(Server *server, const Parsing &parse, Client &client)
{
	if (parse.getArguments().size() == 1)
	 	sendMessage(client.getFD(), server->getHostname(), "461 " + client.getNick() + " PING", "Not enough parameters");
	else if (parse.getArguments()[1].empty())		
	 	sendMessage(client.getFD(), server->getHostname(), "409 " + client.getNick(), "No origin specified");
	else
	 	sendMessage(client.getFD(), server->getHostname(), "PONG " + parse.getArguments()[1], client.getNick());
}

