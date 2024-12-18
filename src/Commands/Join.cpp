/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduvilla <aduvilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 17:19:53 by eschussl          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/12/17 17:51:10 by eschussl         ###   ########.fr       */
=======
/*   Updated: 2024/12/17 18:27:22 by aduvilla         ###   ########.fr       */
>>>>>>> 2aef5294e1965bb4b2a8ba74b2a0886033821d3f
/*                                                                            */
/* ************************************************************************** */

#include "Join.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include <iostream>
#include "Parsing.hpp"
#include "Channel.hpp"
#include "utils.hpp"
#include "Exceptions.hpp"

void Join::execute(Server *server, const Parsing &parse, Client &client)
{
	try
	{
		if (parse.getArguments().size() < 2)
			throw (client.getNick(), parse.getCommand());
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	if (parse.getArguments().size() == 3)
		ifPassword(server, parse, client);
	ifNoPassword(server, parse, client);
}



void Join::ifPassword(Server *server, const Parsing &parse, Client &client)
{
	std::vector<std::string> channels = vsplit(parse.getArguments()[1], ',');
	std::vector<std::string> passwords = vsplit(parse.getArguments()[2], ',');
	std::vector<std::string>::iterator iterPasswords = passwords.begin();
	std::vector<std::string>::iterator iterChannels = channels.begin();
	while (iterChannels != channels.end() && iterPasswords != passwords.end())
	{
		try
		{
			
			Channel *channel = server->findChannel(*iterChannels);
			if (channel == NULL)
				server->createChannel(*iterChannels, client, *iterPasswords);
			else
				channel->addClient(client, *iterPasswords);
			iterChannels++;
			iterPasswords++;
		}
		catch(const serverExceptions& e)
		{
			if (e.getErrorCode() == 403)
				sendMessage(client.getFD(), server->getHostname(), "403" + client.getPrefix() + *iterChannels, "No such channel");
		}
	}
	while (iterChannels != channels.end())
	{
		try
		{
			if (isChannelNameWellFormated(*iterChannels))
				throw serverExceptions(403);
			Channel *channel = server->findChannel(*iterChannels);
			if (channel == NULL)
				server->createChannel(*iterChannels, client);
			else
				channel->addClient(client);
			iterChannels++;
		}
		catch(const serverExceptions& e)
		{
			if (e.getErrorCode() == 403)
				sendMessage(client.getFD(), server->getHostname(), "403" + client.getPrefix() + *iterChannels, "No such channel");
		}
	}
}

void Join::ifNoPassword(Server *server, const Parsing &parse, Client &client)
{
	std::vector<std::string> channels = vsplit(parse.getArguments()[1], ',');
	std::vector<std::string>::iterator iterChannels = channels.begin();
	while (iterChannels != channels.end())
	{
		try
		{
			if (isChannelNameWellFormated(*iterChannels))
				throw serverExceptions(403);
			Channel *channel = server->findChannel(*iterChannels);
			if (channel == NULL)
				server->createChannel(*iterChannels, client);
			else
				channel->addClient(client);
			iterChannels++;
		}
		catch(const serverExceptions& e)
		{
			if (e.getErrorCode() == 403)
				sendMessage(client.getFD(), server->getHostname(), "403" + client.getPrefix() + *iterChannels, "No such channel");
		}
	}
}

		