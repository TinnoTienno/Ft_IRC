/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 14:21:44 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/11 18:23:25 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Nick.hpp"
#include "utils.hpp"
#include <iostream>

void Nick::execute(Server *server, const std::string buffer, Client &client)
{
	int errcode = checkformat(server, buffer);
	std::cout << "Nick::execute : " << buffer << std::endl;
	switch (errcode)
	{
	case 432:
		sendMessage(client.getFD(), server->getHostname(), "432", "Erroneus nickname");
		break;
	case 433:
		sendMessage(client.getFD(), server->getHostname(), "433", "Nickname already in use");
		break;
	default:
		sendMessage(client.getFD(), server->getHostname(), "001 " + client.getNick(), "Bienvenue sur le serveur IRC");
		std::string message = ":"  + client.getPrefix(server->getHostname()) + " NICK :" + buffer + "\r\n";
		send(client.getFD(), message.c_str(), message.size(), 0);
//		sendMessage(client.getFD(), source, "NICK", buffer.substr(buffer.find(" ") + 1, buffer.npos));
		client.setNick(buffer.substr(buffer.find(" ") + 1, buffer.npos));
	}
}

int Nick::checkformat(Server *server, const std::string &line)
{
	if (line.find_first_of("@.:!") != line.npos || !line.find("#") || isdigit(line[0]))
		return 432; // ERR_NONICKNAMEGIVEN
	if (server->findNick(line))
		return 433;
	return -1;
}
