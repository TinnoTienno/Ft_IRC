/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 15:59:44 by eschussl          #+#    #+#             */
/*   Updated: 2025/01/15 16:10:54 by noda             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Ping.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "utils.hpp"
#include "Parsing.hpp"
#include "Numerics.hpp"
#include "serverExceptions.hpp"

/**
 * @brief  Executes the PING command.
 *
 * This function handles the PING command, which checks the connection between the client and server.
 *
 * @param  server : Reference to the server object.
 * @param  parse : Reference to the parsing object containing command arguments.
 * @param  client : Reference to the client object issuing the command.
 */
void Ping::execute(Server &server, const Parsing &parse, Client &client)
{
	if (parse.getArguments().size() == 1)
		server.sendf(&client, NULL, NULL, ERR_NEEDMOREPARAMS, "PING");
	else if (parse.getArguments()[1].empty())		
		server.sendf(&client, NULL, NULL, ERR_NOORIGIN);
	else
		server.sendf(&client, NULL, NULL, PONG, parse.getArguments()[1].c_str());
}

