/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverExceptions.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 16:06:42 by eschussl          #+#    #+#             */
/*   Updated: 2025/01/14 13:45:51 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "serverExceptions.hpp"
#include "utils.hpp"
#include "Numerics.hpp"
#include <iostream>
#include <stdarg.h>
#include "Server.hpp"

int serverExceptions::getErrorCode() const { return m_errorCode; }

serverExceptions::serverExceptions(const int errorCode) : m_errorCode(errorCode) { }

void serverExceptions::sendError(Server &server, Client *dest, Channel *channel,...) const
{
	va_list args;
	va_start(args, channel);
	
	char *str1 = va_arg(args, char *);
	char *str2 = va_arg(args, char *);
	char *str3 = va_arg(args, char *);
	switch(m_errorCode)
	{
		case 401 :
			return str1 ? server.sendf(dest, NULL, NULL, ERR_NOSUCHNICK, str1) : va_end(args);
		case 403 :
			return str1 ? server.sendf(dest, NULL, NULL, ERR_NOSUCHCHANNEL, str1) : va_end(args);
		case 404 :
			return channel ? server.sendf(dest, NULL, channel, ERR_NOSUCHCHANNEL, str1) : va_end(args);
		case 405 :
			if (!channel)
				std::cout << m_errorCode << " no channel was given" << std::endl;
			else
				server.sendf(dest, NULL, channel, ERR_TOOMANYCHANNELS, str1);
			break ;
		case 406 :
			if (!str1)
				std::cout << m_errorCode << " no nickname was given" << std::endl;
			else
				server.sendf(dest, NULL, NULL,  ERR_WASNOSUCHNICK, str1);
			break ;
		case 409 :
			server.sendf(dest, NULL, NULL,  ERR_NOORIGIN);
			break ;
		case 411 :
			if (!str1)
				std::cout << m_errorCode << " no command was given" << std::endl;
			else
				server.sendf(dest, NULL, NULL,  ERR_NORECIPIENT, str1);
			break ;	
		case 412 :
			server.sendf(dest, NULL, NULL,  ERR_NOTEXTTOSEND);
			break ;
		case 417 :
			server.sendf(dest, NULL, NULL,  ERR_INPUTTOOLONG);
			break ;	
		case 421 :
			if (!str1)
				std::cout << m_errorCode << " no command was given" << std::endl;
			else
				server.sendf(dest, NULL, NULL, ERR_UNKNOWNCOMMAND, str1);
			break ;	
		case 422 :
			server.sendf(dest, NULL, NULL, ERR_NOMOTD);
			break ;
		case 431 :
			server.sendf(dest, NULL, NULL,  ERR_NONICKNAMEGIVEN);
			break ;
		case 432 :
			if (!str1)
				std::cout << m_errorCode << " no nickname was given" << std::endl;
			else
				server.sendf(dest, NULL, NULL,  ERR_ERRONEUSNICKNAME);
			break ;	
		case 433 :
			if (!str1)
				std::cout << m_errorCode << " no nickname was given" << std::endl;
			else
				server.sendf(dest, NULL, NULL,  ERR_NICKNAMEINUSE, str1);
			break ;
		case 436 :
			if (!str1 || !str2 || !str3)
				std::cout << m_errorCode << " not enough args were given" << std::endl;
			else
				server.sendf(dest, NULL, NULL,  ERR_NICKCOLLISION, str1, str2, str3);
			break ;	
		case 441 :
			if (!str1 || !channel)
				std::cout << m_errorCode << " not enough args were given" << std::endl;
			else
				server.sendf(dest, NULL, channel,  ERR_USERNOTINCHANNEL, str1);
			break ;
		case 442 :
			if (!channel)
				std::cout << m_errorCode << " no channel was given" << std::endl;
			else
				server.sendf(dest, NULL, channel,  ERR_NOTONCHANNEL);
			break ;
		case 443 :
			if (!str1 || !channel)
				std::cout << m_errorCode << " not enough args were given" << std::endl;
			else
				server.sendf(dest, NULL, channel,  ERR_USERONCHANNEL, str1);
			break ;
		case 451 :
			server.sendf(dest, NULL, NULL,  ERR_NOTREGISTERED);
			break ;
		case 461 :
			if (!str1)
				std::cout << m_errorCode << " no command was given" << std::endl;
			else
				server.sendf(dest, NULL, NULL,  ERR_NEEDMOREPARAMS, str1);
			break ;
		case 462 :
			server.sendf(dest, NULL, NULL,  ERR_ALREADYREGISTERED);
			break ;
		case 464 :
			server.sendf(dest, NULL, NULL,  ERR_PASSWDMISMATCH);
			break ;
		case 465 :
			server.sendf(dest, NULL, NULL,  ERR_YOUREBANNEDCREEP);
			break ;
		case 471 :
			if (!channel)
				std::cout << m_errorCode << " no channel was given" << std::endl;
			else
				server.sendf(dest, NULL, channel,  ERR_CHANNELISFULL);
			break ;
		case 472 :
			if (!str1)
				std::cout << m_errorCode << " no modecharacter was given" << std::endl;
			else
				server.sendf(dest, NULL, NULL,  ERR_UNKNOWNMODE, str1);
			break ;
		case 473 :
			if (!channel)
				std::cout << m_errorCode << " no channel was given" << std::endl;
			else
				server.sendf(dest, NULL, channel, ERR_INVITEONLYCHAN);
			break ;
		case 474 :
			if (!channel)
				std::cout << m_errorCode << " no channel was given" << std::endl;
			else
				server.sendf(dest, NULL, channel, ERR_BANNEDFROMCHAN);
			break ;
		case 475 :
			if (!channel)
				std::cout << m_errorCode << " no channel was given" << std::endl;
			else
				server.sendf(dest, NULL, channel,  ERR_BADCHANNELKEY);
			break ;
		case 476 :
			if (!str1)
				std::cout << m_errorCode << " no channel was given" << std::endl;
			else
				server.sendf(dest, NULL, NULL,  ERR_BADCHANMASK, str1);
			break ;
		case 481 :
			server.sendf(dest, NULL, NULL,  ERR_NOPRIVILEGES);
			break ;
		case 482 :
			if (!channel)
				std::cout << m_errorCode << " no channel was given" << std::endl;
			else
				server.sendf(dest, NULL, channel,  ERR_CHANOPRIVSNEEDED);
			break ;
		case 483 :
			server.sendf(dest, NULL, NULL,  ERR_CANTKILLSERVER);
			break ;
		case 491 :
			server.sendf(dest, NULL, NULL,  ERR_NOOPERHOST);
			break ;
		case 501 :
			server.sendf(dest, NULL, NULL,  ERR_UMODEUNKNOWNFLAG);
			break ;
		case 502 :
			server.sendf(dest, NULL, NULL,  ERR_USERDONTMATCH);
			break ;
		case 524 :
			if (!str1)
				std::cout << m_errorCode << " no subject was given" << std::endl;
			else
				server.sendf(dest, NULL, NULL,  ERR_HELPNOTFOUND, str1);
			break ;
		case 525 :
			if (!str1)
				std::cout << m_errorCode << " no channel was given" << std::endl;
			else
				server.sendf(dest, NULL, NULL,  ERR_INVALIDKEY, str1);
			break ;
		case 696 :
			if (!str1 || !str2 || !str3 || channel)
				std::cout << m_errorCode << " not enough args were given" << std::endl;
			else
				server.sendf(dest, NULL, channel,  ERR_INVALIDMODEPARAM, str1, str2, str3);
			break ;
		default :
			break;
	}
	va_end(args);
}
