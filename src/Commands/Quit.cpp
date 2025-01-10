/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 14:56:08 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/10 16:48:56 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	server.sendLog("Client <" + itoa(client.getFD()) + "> Disconnected");
	client.sendQuitMsg(message);
	server.ClearClient(client);
}

