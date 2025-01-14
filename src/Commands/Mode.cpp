/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 19:15:16 by noda              #+#    #+#             */
/*   Updated: 2025/01/14 15:28:01 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Mode.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Parsing.hpp"
#include "serverExceptions.hpp"
#include "Channel.hpp"
#include "utils.hpp"
#include <cstdlib>
#include <string>
#include <vector>
#include "Numerics.hpp"

void Mode::modeI(Channel &channel, bool status)
{
	channel.getMode()->setInviteOnly(status);
	channel.sendAllMode(status, "i");
}

void Mode::modeT(Channel & channel, bool status)
{
	channel.getMode()->setTopicProtected(status);
	channel.sendAllMode(status, "t");
}
	
void	Mode::modeL(Channel & channel, bool status, const std::string &modeArg)
{
	if (status)
	{
		channel.getMode()->setSizeLimited(status);
		try
		{
			channel.getMode()->setLimitSize(strtous(modeArg)); // verif unsigned int
		}
		catch(const std::exception& e)
		{
			(void) e;
		}
		
		channel.sendAllMode(status, "l " + modeArg);
	}
	else
	{
		channel.getMode()->setSizeLimited(status);
		channel.sendAllMode(status, "l");
	}
}

void	Mode::modeO(Server &server, Channel & channel, Client &source, bool status, const std::string &modeArg)
{
	std::vector<std::string>	tokens = vsplit(modeArg, ',');
	for (size_t i = 0; i < tokens.size(); i++)
	{
		try
		{
			Client * client = channel.getClient(tokens[i]);
			if (!client)
				throw serverExceptions(401);
			if (client && status && !channel.getMode()->isOP(client))
			{
				channel.getMode()->addOP(client);
				channel.sendAllMode(status, "o " + modeArg); // a voir si c'est bien ?
			}
			if (client && !status && channel.getMode()->isOP(client))
			{
				channel.getMode()->removeOP(client);
				channel.sendAllMode(status, "o " + modeArg);
			}
		}
		catch(const serverExceptions& e)
		{
			switch (e.getErrorCode())
			{
				case 401 :
				e.sendError(server, &source, NULL, tokens[i].c_str());
			}
		}
		
		
	}
}
	
void	Mode::modeK(Channel & channel, bool status, const std::string &modeArg)
{
	if (status)
	{
		channel.getMode()->setPasswordProtected(status);
		channel.getMode()->setPassword(modeArg);
		channel.sendAllMode(status, "k");
	}
	else
	{
		channel.getMode()->setPasswordProtected(status);
		channel.sendAllMode(status, "k");
	}
}

void	Mode::modeB(Channel &channel, Client &source)
{
	channel.getMode()->sendBanList(*channel.getServ(), channel, source);
}

void	Mode::modeB(Channel &channel, bool status, const std::string bannedPrefix)
{
	if (status)
		channel.getMode()->setBanned(bannedPrefix);
	else
		channel.getMode()->unsetBanned(bannedPrefix);
	channel.sendAllMode(true, "b " + bannedPrefix);
}
#include <iostream>
void Mode::channelMode(Server &server, Channel &channel, Client &source, const Parsing &parse)
{
	bool status = 0;
	(void) server;
	std::string arg = parse.getArguments()[2];
	bool statusUpdated = 0;
	size_t argIndex = 3;
	for (size_t i = 0; i < arg.size(); i++)
	{
		switch (arg[i])
		{
			case '+' :
				statusUpdated = true;
				status = true;
				break;
			case '-' :
				statusUpdated = true;
				status = false;
				break;
			case 'i' :
				if (statusUpdated)
					Mode::modeI(channel, status);
				break;
			case 't' :
				if (statusUpdated)
					Mode::modeT(channel, status);
				break;
			 case 'k' :
				if (statusUpdated && parse.getArguments().size() > argIndex)
					Mode::modeK(channel, status, parse.getArguments()[argIndex++]);
				else
					throw serverExceptions(461);
				break;
			 case 'o' :
				if (statusUpdated && parse.getArguments().size() > argIndex)
					Mode::modeO(server, channel, source, status, parse.getArguments()[argIndex++]);
				else
					throw serverExceptions(461);
				break;
			case 'l' :
				if (statusUpdated && parse.getArguments().size() > argIndex)
					Mode::modeL(channel, status, parse.getArguments()[argIndex++]);
				else
					throw serverExceptions(461);
			case 'b' :
				if (!statusUpdated)
					Mode::modeB(channel, source);
				else if (parse.getArguments().size() > argIndex)
					Mode::modeB(channel, status, parse.getArguments()[argIndex++]);
				else
					throw serverExceptions(461);
				break;
			default :
					throw serverExceptions(525);
				break;
		}
	}
}

void Mode::execute(Server &server, const Parsing &parse, Client &client)
{
	Channel *chan = NULL;
	try
	{
		if (parse.getArguments().size() < 2)
			throw serverExceptions(461);
		if (parse.getArguments()[1].find_first_of("#&") == parse.getArguments()[1].npos )
			return ;
		chan = server.getChannel(parse.getArguments()[1]);		\
		if (!chan)
			throw serverExceptions(403);
		if (parse.getArguments().size() == 2)
			return server.sendf(&client, NULL, chan, RPL_CHANNELMODEIS, chan->getMode()->modeToStr().c_str());
		if (!chan->getMode()->isOP(&client))
			throw serverExceptions(482);
		Mode::channelMode(server, *chan, client, parse);
	}
	catch(const serverExceptions& e)
	{
		switch (e.getErrorCode())
		{
		case 403 :
			e.sendError(server, &client, NULL, parse.getArguments()[1].c_str());
			break;
		case 461 :
			e.sendError(server, &client, NULL, parse.getArguments()[0].c_str());
		case 482 :
			e.sendError(server, &client, chan);
			break;
		case 525 :
			e.sendError(server, &client, chan, parse.getArguments()[1].c_str(), parse.getArguments()[2].c_str(), "");
			break;
		default:
			break;
		}
	}
}
