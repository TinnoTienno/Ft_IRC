/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   authentificationServer.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 16:13:54 by aduvilla          #+#    #+#             */
/*   Updated: 2024/12/13 16:11:53 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "utils.hpp"
#include <iostream>
#include "Nick.hpp"
#include "Parsing.hpp"

bool Server::checkAuth(Client &client, const std::string &buffer)
{
	size_t j = 0;
	if (client.getAuth() == 1)
		return 1;
	for (size_t i = 0; i < buffer.size() ; i = j + 1)
	{
		j = buffer.find("\n", i + 1);
		std::string line = buffer.substr(i, j - i - 1);
		Parsing parse(line);
		if (parse.getCommand() == "NICK" && Nick::nickErrorCode(this, client, parse))
			client.setNick(getNextGuest());
		if (parse.getCommand() == "PASS" && parse.getArguments()[1] == m_pass)
		{
			client.setAuth(true);
			std::cout << GRE << "Client <" << client.getFD() << "> is now authentified" << WHI << std::endl;
		}
		else if (parse.getCommand() == "PASS")
		{
			std::cout << "|" << parse.getArguments()[1] << "|" << std::endl;
			std::cout << RED << "Client <" << client.getFD() << "> has a wrong password" << WHI << std::endl;
			client.kill("password is wrong");
			ClearClient(client);
			return 0;
		}
		if (parse.getCommand() == "USER" && !userErrorCode(client, parse))
		{
			if (client.getAuth() == 1  ||( m_pass == "" && client.getAuth() == false))
			{
				client.setAuth(true);
				std::cout << GRE << "Client <" << client.getFD() << "> is now authentified" << WHI << std::endl;
				client.connect(this);
			}
			return 0;
		}
	}
	if (m_pass != "" && client.getAuth() == false)
	    std::cout << RED << "Client <" << client.getFD() << "> has not set a password" << WHI << std::endl;
	return 0;
}
