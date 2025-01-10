/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 15:59:44 by eschussl          #+#    #+#             */
/*   Updated: 2025/01/08 17:07:02 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Ping.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "utils.hpp"
#include "Parsing.hpp"
#include "serverExceptions.hpp"

void Ping::execute(Server &server, const Parsing &parse, Client &client)
{
	if (parse.getArguments().size() == 1)
	 	sendMessage(client.getFD(), server, "461 " + client.getNickname() + " PING", "Not enough parameters");
	else if (parse.getArguments()[1].empty())		
	 	sendMessage(client.getFD(), server, "409 " + client.getNickname(), "No origin specified");
	else
	 	sendMessage(client.getFD(), server, "PONG " + parse.getArguments()[1], client.getNickname());
}

