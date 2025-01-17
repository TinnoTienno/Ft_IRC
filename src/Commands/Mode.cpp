/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 19:15:16 by noda              #+#    #+#             */
/*   Updated: 2025/01/15 16:21:33 by aduvilla         ###   ########.fr       */
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

/**
 * @brief Sets the invite-only mode for a channel.
 * 
 * @param server The server instance.
 * @param channel The channel to modify.
 * @param source The client issuing the command.
 * @param status The new status of the invite-only mode.
 */
void Mode::modeI(Server &server,Channel &channel, Client &source, bool status)
{
	std::string strStat = status ? "+i" : "-i";
	channel.getMode()->setInviteOnly(status);
	channel.sendAllMsg(&server, &source, strStat, eMode);
}

/**
 * @brief Sets the topic-protected mode for a channel.
 * 
 * @param server The server instance.
 * @param channel The channel to modify.
 * @param source The client issuing the command.
 * @param status The new status of the topic-protected mode.
 */
void Mode::modeT(Server & server,Channel & channel, Client &source, bool status)
{
	std::string strStat = status ? "+t" : "-t";
	channel.getMode()->setTopicProtected(status);
	channel.sendAllMsg(&server, &source, strStat, eMode);
}
	
/**
 * @brief Sets the size limit for a channel.
 * 
 * @param server The server instance.
 * @param channel The channel to modify.
 * @param source The client issuing the command.
 * @param status The new status of the size limit mode.
 * @param modeArg The size limit value.
 */
void	Mode::modeL(Server & server, Channel & channel, Client & source, bool status, const std::string &modeArg)
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
		channel.sendAllMsg(&server, &source, "+l " + modeArg, eMode);
	}
	else
	{
		channel.getMode()->setSizeLimited(status);
		channel.sendAllMsg(&server, &source, "-l ", eMode);
	}
}

/**
 * @brief Sets the operator status for clients in a channel.
 * 
 * @param server The server instance.
 * @param channel The channel to modify.
 * @param source The client issuing the command.
 * @param status The new status of the operator mode.
 * @param modeArg The list of clients to modify.
 */
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
				channel.sendAllMsg(&server, &source, "+o " + modeArg, eMode);
			}
			if (client && !status && channel.getMode()->isOP(client))
			{
				channel.getMode()->removeOP(client);
				channel.sendAllMsg(&server, &source, "-o " + modeArg, eMode);
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
	
/**
 * @brief Sets the password protection for a channel.
 * 
 * @param server The server instance.
 * @param channel The channel to modify.
 * @param source The client issuing the command.
 * @param status The new status of the password protection mode.
 * @param modeArg The password value.
 */
void	Mode::modeK(Server & server, Channel & channel, Client & source, bool status, const std::string &modeArg)
{
	if (status)
	{
		channel.getMode()->setPasswordProtected(status);
		channel.getMode()->setPassword(modeArg);
		channel.sendAllMsg(&server, &source, "+k ", eMode);
	}
	else
	{
		channel.getMode()->setPasswordProtected(status);
		channel.sendAllMsg(&server, &source, "-k ", eMode);
	}
}

/**
 * @brief Sends the ban list for a channel.
 * 
 * @param server The server instance.
 * @param channel The channel to query.
 * @param source The client issuing the command.
 */
void	Mode::modeB(Server & server, Channel &channel, Client &source)
{
	channel.getMode()->sendBanList(server, channel, source);
}

/**
 * @brief Sets the ban status for a client in a channel.
 * 
 * @param server The server instance.
 * @param channel The channel to modify.
 * @param source The client issuing the command.
 * @param status The new status of the ban mode.
 * @param bannedPrefix The prefix of the client to ban/unban.
 */
void	Mode::modeB(Server & server, Channel &channel, Client & source, bool status, const std::string bannedPrefix)
{
	std::string strStat = status ? "+b" : "-b";
	if (status)
		channel.getMode()->setBanned(bannedPrefix);
	else
		channel.getMode()->unsetBanned(bannedPrefix);
	channel.sendAllMsg(&server, &source, strStat + bannedPrefix, eMode);
}

/**
 * @brief Handles channel mode changes.
 * 
 * @param server The server instance.
 * @param channel The channel to modify.
 * @param source The client issuing the command.
 * @param parse The parsed command.
 */
void Mode::channelMode(Server &server, Channel &channel, Client &source, const Parsing &parse)
{
	bool status = true;
	(void) server;
	std::string arg = parse.getArguments()[2];
	size_t argIndex = 3;
	for (size_t i = 0; i < arg.size(); i++)
	{
		switch (arg[i])
		{
			case '+' :
				status = true;
				break;
			case '-' :
				status = false;
				break;
			case 'i' :
					Mode::modeI(server,channel, source, status);
				break;
			case 't' :
					Mode::modeT(server, channel, source, status);
				break;
			 case 'k' :
				if (parse.getArguments().size() > argIndex)
					Mode::modeK(server, channel,  source, status, parse.getArguments()[argIndex++]);
				else
					throw serverExceptions(461);
				break;
			 case 'o' :
				if (parse.getArguments().size() > argIndex)
					Mode::modeO(server, channel, source, status, parse.getArguments()[argIndex++]);
				else
					throw serverExceptions(461);
				break;
			case 'l' :
				if (parse.getArguments().size() > argIndex)
					Mode::modeL(server, channel, source, status, parse.getArguments()[argIndex++]);
				else
					throw serverExceptions(461);
				break;
			case 'b' :
				if (parse.getArguments().size() > argIndex)
					Mode::modeB(server, channel, source, status, parse.getArguments()[argIndex++]);
				else
					Mode::modeB(server, channel, source);
				break;
			default :
					throw serverExceptions(525);
				break;
		}
	}
}

/**
 * @brief Executes the MODE command.
 * 
 * @param server The server instance.
 * @param parse The parsed command.
 * @param client The client issuing the command.
 */
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
		{
			return server.sendf(&client, NULL, chan, RPL_CHANNELMODEIS, chan->getMode()->modeToStr().c_str());
		}
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
			break;
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
