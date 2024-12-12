/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   authentificationServer.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 16:13:54 by aduvilla          #+#    #+#             */
/*   Updated: 2024/12/12 15:54:39 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "utils.hpp"
#include <iostream>
#include "Nick.hpp"

int Server::nickErrorCode(Client &client, const std::string &buffer)
{
	size_t nick = buffer.find("NICK");
	nick = buffer.find(' ', nick) + 1;
	size_t nick2 = buffer.find(13, nick);
	std::string  nickname = buffer.substr(nick, nick2 - nick);
	std::cout << "nick |" << nickname << "|" << std::endl;
	if (!nickname.size())
		return 431;
	if (findNick(nickname))
		return 433;
	if (!isNickFormatted(nickname))
		return 432;
	client.setNick(nickname);
	return 0;
}


bool Server::checkAuth(Client &client, const std::string &buffer)
{

	if (client.getAuth() == 1)
		return 1;
	if (buffer.find("USER") == buffer.npos)
	    return 0;
	std::cout << "checkAuth buffer " << buffer << std::endl;
	if (nickErrorCode(client, buffer))
		client.setNick(getNextGuest());
	if (!userErrorCode(client, buffer))
		return 0;
	size_t pass = buffer.find("PASS");
	if (pass == buffer.npos)
	{
	    std::cout << RED << "Client <" << client.getFD() << "> has not set a password" << WHI << std::endl;
		return 0;
	}
	pass = buffer.find(' ', pass) + 1;
	size_t pass2 = buffer.find(13, pass);
	std::string passwd = buffer.substr(pass, pass2 - pass);
	std::cout << "pass : |" << passwd << "|" << std::endl;
	if (!passwd.compare(m_pass))
	{
		client.setAuth(true);
		std::cout << GRE << "Client <" << client.getFD() << "> is now authentified" << WHI << std::endl;
		client.connect(this);
		return 0;
	}
	std::cout << RED << "Client <" << client.getFD() << "> has a wrong password" << WHI << std::endl;
	client.kill("password is wrong");
	ClearClients(client.getFD());
	return 0;
}
