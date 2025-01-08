/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 14:56:08 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/08 17:07:57 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Quit.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "utils.hpp"
#include "Parsing.hpp"
#include "serverExceptions.hpp"

void Quit::execute(Server &server, const Parsing &parse, Client &client)
{
	std::string	message;
	if (parse.getArguments().size() < 2)
		message = "";
	else
		message = parse.getArguments()[1];
	client.sendQuitMsg(&server, message);
	sendf(&server, &client, ":%h ERROR :Closing Link: %c (Quit: %m)", client.getNickname().c_str(), message.c_str()); 
	server.sendLog("Client <" + itoa(client.getFD()) + "> Disconnected");
	server.ClearClient(client);
}

