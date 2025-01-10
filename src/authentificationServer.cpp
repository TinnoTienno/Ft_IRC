/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   authentificationServer.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 16:13:54 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/10 12:52:56 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "utils.hpp"
#include "Nick.hpp"
#include "Parsing.hpp"
#include "Err.hpp"
#include "serverExceptions.hpp"

bool Server::checkAuth(Client &client, const std::string &buffer)
{
	size_t j = 0;
	if (client.getAuth() == 1 && !client.getUser().empty() )
		return true;
	for (size_t i = 0; i < buffer.size() ; i = j + 1)
	{
		j = buffer.find("\n", i + 1);
		std::string line = buffer.substr(i, j - i - 1);
		sendLog(itoa(client.getFD()) + " >> " + line);
		Parsing parse(line);
		if (parse.getCommand() == "NICK")
		{
			if (Nick::parseError(*this, parse, client))
				client.setNick(getNextGuest());
		}
		else if (parse.getCommand() == "PASS")
		{
			if (parse.getArguments()[1] == m_pass)
				client.setAuth(true);
			else
			{
				client.kill(this, "password is wrong");
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
			sendf(this, &client, ERR_NOTREGISTERED, parse.getCommand().c_str());
	}
	if (m_pass != "" && client.getAuth() == false && !client.getUser().empty())
		sendLog("Client <" + itoa(client.getFD()) + "> has not set a password");
	return false;
}
