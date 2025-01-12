/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 19:15:16 by noda              #+#    #+#             */
/*   Updated: 2025/01/12 13:40:49 by aduvilla         ###   ########.fr       */
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
#include "Rpl.hpp"

void Mode::modeI(Channel &channel, bool status)
{
	channel.getMode()->setInviteOnly(status);
//	channel.m_serv->sendLog(clientgetName() + "'s invite mode was set to " + itoa(status));
	channel.sendAllMode(status, "i");
}

void Mode::modeT(Channel channel, bool status)
{
	channel.getMode()->setTopicProtected(status);
	// message dans le log ?????
	channel.sendAllMode(status, "t");
}
	
void Mode::channelMode(Server &server, Channel &channel, Client &source, const Parsing &parse)
{
	bool status;
	(void) source;
	if (parse.getArguments().size() == 2)
		return server.sendf(&source, &source, &channel, RPL_CHANNELMODEIS, channel.modeToStr().c_str());
	std::string arg = "";
	for (size_t i = 2; i < parse.getArguments().size(); i++)
		arg += parse.getArguments()[i] + ' ';
	server.sendLog("Debug : arg :" + arg);
	if (arg[0] == '+')
		status = 1;
	else if (arg[0] == '-')
		status = 0;
	else
		return server.sendLog((std::string)"Error in channelmode, mode effect not recognized : " + arg[0]);
	for (size_t i = 1; arg[i] != ' '; i++)
	{
		switch (arg[i])
		{
			case 'i' :
				return Mode::modeI(channel, status);
			case 't' :
				return Mode::modeT(channel, status);
			// case 'k' :
			// 	std::string password = "";
			// 	if (arg.find(' ') != arg.npos)
			// 		password = arg.substr(arg.find(' '), arg.find(' ', arg.find(' ') + 1) - arg.find(' '));
			// 	Mode::modeK(server, channel, source, status, password);
			// case 'o' :
			// 	if (arg.find(' ') != arg.npos)
			// case 'l' :

			// default :
			
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
		if (chan)
			Mode::channelMode(server, *chan, client, parse);
		else
			throw serverExceptions(403);
	}
	catch(const serverExceptions& e)
	{
		switch (e.getErrorCode())
		{
		case 403 :
			/* code */
			break;
		case 324 :
			/* code */
			break;
		case 329 :
			/* code */
			break;
		case 482 :
			/* code */
			break;
		default:
			break;
		}
	}
}
