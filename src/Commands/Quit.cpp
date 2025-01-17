/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 14:56:08 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/14 00:37:14 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Quit.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "utils.hpp"
#include "Parsing.hpp"
#include "serverExceptions.hpp"

/**
 * @brief  Executes the QUIT command.
 *
 * This function handles the QUIT command, which disconnects a client from the server.
 *
 * @param  server : Reference to the server object.
 * @param  parse : Reference to the parsing object containing command arguments.
 * @param  client : Reference to the client object issuing the command.
 */
void Quit::execute(Server &server, const Parsing &parse, Client &client)
{
	std::string	message;
	if (parse.getArguments().size() < 2)
		message = "";
	else
		message = parse.getArguments()[1];
	server.sendLog("Client <" + itoa(client.getFD()) + "> Disconnected");
	client.sendQuitMsg(message);
	server.ClearClient(client);
}
