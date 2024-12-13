/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 15:59:44 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/13 14:02:18 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Ping.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "utils.hpp"
#include "Parsing.hpp"

// static int pingErrorCode(const std::string &buffer)
// {
// 	if (buffer.empty())
// 		return 409;
// 	// if (noparam)
// 	// 	return 461;
// 	return 0;
// }

void Ping::execute(Server *server, const Parsing &parse, Client &client)
{
	(void) server;
	(void) parse;
	(void) client;
	// int errorCode = pingErrorCode(buffer);
	// // std::cout << "Nick::execute : " << buffer << std::endl;
	// switch (errorCode)
	// {
	// case 409:
	// 	sendMessage(client.getFD(), server->getHostname(), "409 " + client.getNick(), "No origin specified");
	// 	break;
	// case 461:
	// 	sendMessage(client.getFD(), server->getHostname(), "461 " + client.getNick() + " PING", "Not enough parameters");
	// 	break;
	// default:
	// 	sendMessage(client.getFD(), server->getHostname(), "PONG " + client.getNick(), buffer);
	// }
}

