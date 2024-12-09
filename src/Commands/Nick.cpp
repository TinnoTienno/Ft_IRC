/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 14:21:44 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/09 18:43:39 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Nick.hpp"
#include <iostream>

void Nick::execute(Server *server, const std::string buffer, Client &client)
{
	int errcode = checkformat(server, buffer);
	std::cout << "Nick::execute : " << buffer << std::endl;
	switch (errcode)
	{
	case 432:
		server->sendMsg(client, "Erroneus nickname", "432");
		break;
	case 433:
		server->sendMsg(client, "Nickname already in use", "433");
		break;
	default:
		server->sendMsg(client, client.getNick() + " :Bienvenue sur le serveur IRC", "001");
		server->sendMsg(client, "!" + client.getUser() + "@" + server->getHostname() + " NICK :" + buffer.substr(buffer.find(" ") + 1, buffer.npos), "");
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