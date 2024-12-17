/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivMsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduvilla <aduvilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 19:39:53 by aduvilla          #+#    #+#             */
/*   Updated: 2024/12/17 14:17:50 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PrivMsg.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "Parsing.hpp"
#include "utils.hpp"

/*
static std::vector<std::string>	vsplit(const std::string & str, char delimiter)
{
	std::vector<std::string>	result;
	std::string::size_type		start = 0;
	std::string::size_type		end = 0;

	while ((end = str.find(delimiter, start)) != std::string::npos)
	{
		result.push_back(str.substr(start, end - start));
		start = end + 1;
	}
	result.push_back(str.substr(start));
	return result;
}
*/

static std::vector<std::string>	vsplit(const std::string & str, char delimiter)
{
	std::vector<std::string>	result;
	std::string					token;

	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] == delimiter && !token.empty())
		{
			result.push_back(token);
			token.clear();
		}
		else if (str[i] != delimiter)
			token += str[i];  
	}
	if (!token.empty())
		result.push_back(token);
	return result;
}

void	PrivMsg::execute(Server *server, const Parsing &parse, Client &client)
{
	if (parse.getArguments()[1].find(":", 0) == 0)
		sendMessage(client.getFD(), server->getHostname(), "411 " + client.getNick(), "No recipient given PRIVMSG");
	else if (parse.getArguments().size() == 2)
		sendMessage(client.getFD(), server->getHostname(), "412 " + client.getNick(), "No text to send");
	std::vector<std::string>	targets = vsplit(parse.getArguments()[1], ',');
	for (size_t i = 0; i < targets.size(); i++)
	{
		if (targets[i].find('#') == 0)
		{
			Channel	*chan = server->getChannel(targets[i].substr(1));
			if (!chan)
				sendMessage(client.getFD(), server->getHostname(), "401 " + client.getNick() + parse.getArguments()[1], "No such nick/channel");
			else
				chan->sendAllMsg(parse.getArguments()[2]); // !!!! il faut aussi envoyer le client actuel pour la source du msg
		}
		else
	  	{
			Client *user = server->getClient( targets[i]);
			if (!user)
				sendMessage(client.getFD(), server->getHostname(), "401 " + client.getNick() + parse.getArguments()[1], "No such nick/channel");
			else
				sendMessage(user->getFD(), client.getPrefix(), "PRIVMSG " + targets[i], parse.getArguments()[2]);
		}
	}
//	else if (no such channel)
//		sendMessage(client.getFD(), server->getHostname(), "404 " + client.getNick() + parse.getArguments()[1], "Cannot send to channel");
}
