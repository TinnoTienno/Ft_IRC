/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 19:15:16 by noda              #+#    #+#             */
/*   Updated: 2025/01/13 17:10:02 by eschussl         ###   ########.fr       */
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
#include "Rpl.hpp"

void Mode::modeI(Channel &channel, bool status)
{
	channel.getMode()->setInviteOnly(status);
//	channel.m_serv->sendLog(clientgetName() + "'s invite mode was set to " + itoa(status));
	channel.sendAllMode(status, "i");
}

void Mode::modeT(Channel & channel, bool status)
{
	channel.getMode()->setTopicProtected(status);
	// message dans le log ?????
	channel.sendAllMode(status, "t");
}
	
void	Mode::modeL(Channel & channel, bool status, const Parsing & parse)
{
	if (status && parse.getArguments().size() > 3)
	{
		channel.getMode()->setSizeLimited(status);
		channel.getMode()->setLimitSize(std::atoi(parse.getArguments()[3].c_str())); // verif unsigned int
		channel.sendAllMode(status, "l");
	}
	else if (!status)
	{
		channel.getMode()->setSizeLimited(status);
		channel.sendAllMode(status, "l");
	}
}

void	Mode::modeO(Channel & channel, bool status, const std::string & modeArg)
{
	std::vector<std::string>	tokens = vsplit(modeArg, ',');
	for (size_t i = 0; i < tokens.size(); i++)
	{
		Client * client = channel.getClient(tokens[i]);
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
}

void	Mode::modeK(Channel & channel, bool status, const Parsing & parse)
{
	if (status && parse.getArguments().size() > 3)
	{
		channel.getMode()->setPasswordProtected(status);
		channel.getMode()->setPassword(parse.getArguments()[3]);
		channel.sendAllMode(status, "k");
	}
	else if (!status)
	{
		channel.getMode()->setPasswordProtected(status);
		channel.sendAllMode(status, "k");
	}
}

void	Mode::modeD(Channel &channel, Client &source)
{
	channel.getMode()->sendBanList(*channel.getServ(), channel, source);
}

void	Mode::modeD(Channel &channel, bool status, const std::string bannedPrefix)
{
	if (status)
		channel.getMode()->setBanned(bannedPrefix);
	else
		channel.getMode()->unsetBanned(bannedPrefix);
	channel.sendAllMode(true, "b " + bannedPrefix);
}
void Mode::channelMode(Server &server, Channel &channel, Client &source, const Parsing &parse)
{
	bool status;
	bool statusUpdated = 0;
	if (parse.getArguments().size() == 2)
		return server.sendf(&source, &source, &channel, RPL_CHANNELMODEIS, channel.modeToStr().c_str());
	std::string arg = parse.getArguments()[2];
//	server.sendLog("Debug : arg :" + arg);
	for (size_t i = 1; i < arg.size(); i++)
	{
		switch (arg[i])
		{
			case '+' :
				statusUpdated = 1;
				status = 1;
				break;
			case '-' :
				statusUpdated = 1;
				status = 0;
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
				if (statusUpdated)
					Mode::modeK(channel, status, parse.getArguments()[3]);
				break;
			 case 'o' :
				if (statusUpdated && parse.getArguments().size() > 3)
					Mode::modeO(channel, status, parse.getArguments()[3]);
				break;
			case 'l' :
				if (statusUpdated && parse.getArguments().size() > 3)
					Mode::modeL(channel, status, parse.getArguments()[3]);
				break;
			case 'd' :
				if (!statusUpdated || !(parse.getArguments().size() > 3))
					Mode::modeD(channel, source);
				else
					Mode::modeD(channel, status, parse.getArguments()[3]);
				break;
			default :
				break;
		}
	}
}

void Mode::execute(Server &server, const Parsing &parse, Client &client)
{
	try
	{
		if (parse.getArguments().size() < 2)
			throw serverExceptions(461);

		Channel *chan = server.getChannel(parse.getArguments()[1]);
		if (!chan)
			throw serverExceptions(403);
		if (!chan->getMode()->isOP(&client))
			throw serverExceptions(482);
		Mode::channelMode(server, *chan, client, parse);
	}
	catch(const serverExceptions& e)
	{
		switch (e.getErrorCode())
		{
		case 403 :
			e.sendError(server, &client, NULL, parse.getArguments()[2].c_str());
			break;
		case 482 :
			/* code */
			break;
		default:
			break;
		}
	}
}
