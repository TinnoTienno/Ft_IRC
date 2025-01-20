/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   authentificationServer.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 16:13:54 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/17 13:53:06 by noda             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "utils.hpp"
#include "Nick.hpp"
#include "Parsing.hpp"
#include "Numerics.hpp"
#include "serverExceptions.hpp"

/**
 * @brief Checks the authentication of a client.
 * 
 * This function processes the authentication commands (NICK, PASS, USER) sent by the client.
 * 
 * @param client The client object.
 * @param buffer The buffer containing the client's input.
 * @return true if the client is authenticated, false otherwise.
 */
bool Server::checkAuth(Client &client, const std::string &buffer)
{
	size_t j = 0;
	if (client.getAuth() == 1 && !client.getUsername().empty() )
		return true;
	for (size_t i = 0; i < buffer.size() ; i = j + 1)
	{
		j = buffer.find("\n", i + 1);
		std::string line = buffer.substr(i, j - i - 1);
		sendLog(itoa(client.getFD()) + " >> " + line);
		Parsing parse(line);
		if (parse.getCommand() == "NICK")
		{
			Nick::parseError(*this, parse, client);
		}
		else if (parse.getCommand() == "PASS")
		{
			if (parse.getArguments().size() == 1)
				return this->sendf(&client, NULL, NULL,  ERR_NONICKNAMEGIVEN), false;
			if (parse.getArguments()[1] == m_pass)
				client.setAuth(true);
			else
			{
				client.kill(*this, "password is wrong");
				ClearClient(client);
				return false;
			}
		}
		else if (parse.getCommand() == "USER" && !userErrorCode(client, parse))
		{
			if (client.getAuth() == 1  || (m_pass.empty() && client.getAuth() == false))
			{
				client.setAuth(true);
				sendLog("Client <" + itoa(client.getFD()) + "> is now authentified");
				client.connect(*this);
			}
			return false;
		}
		else if (parse.getCommand() == "CAP")
			;
		else
			this->sendf(&client, NULL, NULL, ERR_NOTREGISTERED, parse.getCommand().c_str());
	}
	if (m_pass != "" && client.getAuth() == false && !client.getUsername().empty())
		sendLog("Client <" + itoa(client.getFD()) + "> has not set a password");
	return false;
}
