/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   authentificationServer.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 16:13:54 by aduvilla          #+#    #+#             */
/*   Updated: 2024/12/16 18:27:23 by aduvilla         ###   ########.fr       */
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
	if (client.getAuth() == 1 && !client.getUser().empty() )
		return 1;
	for (size_t i = 0; i < buffer.size() ; i = j + 1)
	{
		j = buffer.find("\n", i + 1);
		std::string line = buffer.substr(i, j - i - 1);
		Parsing parse(line);
		if (parse.getCommand() == "NICK")
		{
			if (Nick::errorCode(this, parse, client))
				client.setNick(getNextGuest());
		}
		else if (parse.getCommand() == "PASS")
		{
			if (parse.getArguments()[1] == m_pass)
				client.setAuth(true);
			else
			{
				client.kill("password is wrong");
				ClearClient(client);
				return 0;
			}
		}
		else if (parse.getCommand() == "USER" && !userErrorCode(client, parse))
		{
			if (client.getAuth() == 1  || (m_pass.empty() && client.getAuth() == false))
			{
				client.setAuth(true);
				std::cout << GRE << "Client <" << client.getFD() << "> is now authentified" << WHI << std::endl;
				client.connect(this);
			}
			return 0;
		}
		else if (parse.getCommand() == "CAP")
			;
		else
			sendMessage(client.getFD(), this->getHostname(), "451 * " + parse.getCommand(), "You must finish connecting with another nickname first.");
	}
	if (m_pass != "" && client.getAuth() == false && !client.getUser().empty())
	    std::cout << RED << "Client <" << client.getFD() << "> has not set a password" << WHI << std::endl;
	return 0;
}
