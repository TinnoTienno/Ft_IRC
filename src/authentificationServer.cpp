/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   authentificationServer.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduvilla <aduvilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 16:13:54 by aduvilla          #+#    #+#             */
/*   Updated: 2024/12/11 17:25:16 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>

bool Server::checkNick(Client &client, const std::string &buffer)
{
	size_t nick = buffer.find("NICK");
	nick = buffer.find(' ', nick) + 1;
	size_t nick2 = buffer.find(13, nick);
	std::string  nickname = buffer.substr(nick, nick2 - nick);
	std::cout << "nick |" << nickname << "|" << std::endl;
	client.setNick(nickname);
	for (size_t i = 0; i < m_vClients.size(); i++)
	{
		if (m_vClients[i].getNick() == nickname && &m_vClients[i] != &client)
		{
			std::cout << RED << "Client <" << client.getFD() << "> nickname already used" << WHI << std::endl;
			client.kill("nickname already taken");
			ClearClients(client.getFD());
			return 0;
		}
	}
	return 1;
}

bool Server::checkUser(Client &client, const std::string &buffer)
{
	size_t user = buffer.find("USER");
	user = buffer.find(' ', user) + 1;
	size_t user2 = buffer.find(' ', user);
	std::string  username = buffer.substr(user, user2 - user);
	std::cout << "username |" << username << "|" << std::endl;
	client.setUser(username);
	return 1;
}

bool Server::checkAuth(Client &client, const std::string &buffer)
{

	if (client.getAuth() == 1)
		return 1;
	if (buffer.find("USER") == buffer.npos)
	    return 0;
	std::cout << "checkAuth buffer " << buffer << std::endl;
	if (!checkNick(client, buffer))
		return 0;
	if (!checkUser(client, buffer))
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
