/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverExceptions.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 16:06:42 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/19 00:02:31 by noda             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "serverExceptions.hpp"
#include "utils.hpp"
#include "Err.hpp"
#include <iostream>
#include <stdarg.h>

int serverExceptions::getErrorCode() const { return m_errorCode; }

serverExceptions::serverExceptions(const int errorCode) : m_errorCode(errorCode) { }

void serverExceptions::sendError(Server *server, Client *dest, ...) const
{
	if (!server || !dest)
		return ;
	va_list args;
	va_start(args, dest);
	
	char *str1 = va_arg(args, char *);
	std::cout << str1 << std::endl;
	char *str2 = va_arg(args, char *);
	char *str3 = va_arg(args, char *);
	char *str4 = va_arg(args, char *);
	switch(m_errorCode)
	{
		case 401 :
			if (!str1)
				std::cout << m_errorCode << " no nickname was given" << std::endl;
			else
				sendf(server, dest,  ERR_NOSUCHNICK, str1);
			break ;
		case 403 :
			if (!str1)
				std::cout << m_errorCode << " no channel was given" << std::endl;
			else
				sendf(server, dest,  ERR_NOSUCHCHANNEL, str1);
			break ;
		case 404 :
			if (!str1)
				std::cout << m_errorCode << " no channel was given" << std::endl;
			else
				sendf(server, dest,  ERR_CANNOTSENDTOCHAN, str1);
			break ;
		case 405 :
			if (!str1)
				std::cout << m_errorCode << " no channel was given" << std::endl;
			else
				sendf(server, dest,  ERR_TOOMANYCHANNELS, str1);
			break ;
		case 406 :
			if (!str1)
				std::cout << m_errorCode << " no nickname was given" << std::endl;
			else
				sendf(server, dest,  ERR_WASNOSUCHNICK, str1);
			break ;
		case 409 :
			sendf(server, dest,  ERR_NOORIGIN);
			break ;
		case 411 :
			if (!str1)
				std::cout << m_errorCode << " no command was given" << std::endl;
			else
				sendf(server, dest,  ERR_NORECIPIENT, str1);
			break ;	
		case 412 :
			sendf(server, dest,  ERR_NOTEXTTOSEND);
			break ;
		case 417 :
			sendf(server, dest,  ERR_INPUTTOOLONG);
			break ;	
		case 421 :
			if (!str1)
				std::cout << m_errorCode << " no command was given" << std::endl;
			else
				sendf(server, dest,  ERR_UNKNOWNCOMMAND, str1);
			break ;	
		case 422 :
			sendf(server, dest,  ERR_NOMOTD);
			break ;
		case 431 :
			sendf(server, dest,  ERR_NONICKNAMEGIVEN);
			break ;
		case 432 :
			if (!str1)
				std::cout << m_errorCode << " no nickname was given" << std::endl;
			else
				sendf(server, dest,  ERR_ERRONEUSNICKNAME, str1);
			break ;	
		case 433 :
			if (!str1)
				std::cout << m_errorCode << " no nickname was given" << std::endl;
			else
				sendf(server, dest,  ERR_NICKNAMEINUSE, str1);
			break ;
		case 436 :
			if (!str1 || !str2 || !str3)
				std::cout << m_errorCode << " not enough args were given" << std::endl;
			else
				sendf(server, dest,  ERR_NICKCOLLISION, str1, str2, str3);
			break ;	
		case 441 :
			if (!str1 || !str2)
				std::cout << m_errorCode << " not enough args were given" << std::endl;
			else
				sendf(server, dest,  ERR_USERNOTINCHANNEL, str1, str2);
			break ;
		case 442 :
			if (!str1)
				std::cout << m_errorCode << " no channel was given" << std::endl;
			else
				sendf(server, dest,  ERR_NOTONCHANNEL, str1);
			break ;
		case 443 :
			if (!str1 || !str2)
				std::cout << m_errorCode << " not enough args were given" << std::endl;
			else
				sendf(server, dest,  ERR_USERONCHANNEL, str1, str2);
			break ;
		case 451 :
			sendf(server, dest,  ERR_NOTREGISTERED);
			break ;
		case 461 :
			if (!str1)
				std::cout << m_errorCode << " no command was given" << std::endl;
			else
				sendf(server, dest,  ERR_NEEDMOREPARAMS, str1);
			break ;
		case 462 :
			sendf(server, dest,  ERR_ALREADYREGISTERED);
			break ;
		case 464 :
			sendf(server, dest,  ERR_PASSWDMISMATCH);
			break ;
		case 465 :
			sendf(server, dest,  ERR_YOUREBANNEDCREEP);
			break ;
		case 471 :
			if (!str1)
				std::cout << m_errorCode << " no channel was given" << std::endl;
			else
				sendf(server, dest,  ERR_CHANNELISFULL, str1);
			break ;
		case 472 :
			if (!str1)
				std::cout << m_errorCode << " no modechararcter was given" << std::endl;
			else
				sendf(server, dest,  ERR_UNKNOWNMODE, str1);
			break ;
		case 473 :
			if (!str1)
				std::cout << m_errorCode << " no channel was given" << std::endl;
			else
				sendf(server, dest,  ERR_INVITEONLYCHAN, str1);
			break ;
		case 474 :
			if (!str1)
				std::cout << m_errorCode << " no channel was given" << std::endl;
			else
				sendf(server, dest,  ERR_BANNEDFROMCHAN, str1);
			break ;
		case 475 :
			if (!str1)
				std::cout << m_errorCode << " no channel was given" << std::endl;
			else
				sendf(server, dest,  ERR_BADCHANNELKEY, str1);
			break ;
		case 476 :
			if (!str1)
				std::cout << m_errorCode << " no channel was given" << std::endl;
			else
				sendf(server, dest,  ERR_BADCHANMASK, str1);
			break ;
		case 481 :
			sendf(server, dest,  ERR_NOPRIVILEGES);
			break ;
		case 482 :
			if (!str1)
				std::cout << m_errorCode << " no channel was given" << std::endl;
			else
				sendf(server, dest,  ERR_CHANOPRIVSNEEDED, str1);
			break ;
		case 483 :
			sendf(server, dest,  ERR_CANTKILLSERVER);
			break ;
		case 491 :
			sendf(server, dest,  ERR_NOOPERHOST);
			break ;
		case 501 :
			sendf(server, dest,  ERR_UMODEUNKNOWNFLAG);
			break ;
		case 502 :
			sendf(server, dest,  ERR_USERDONTMATCH);
			break ;
		case 524 :
			if (!str1)
				std::cout << m_errorCode << " no subject was given" << std::endl;
			else
				sendf(server, dest,  ERR_HELPNOTFOUND, str1);
			break ;
		case 525 :
			if (!str1)
				std::cout << m_errorCode << " no channel was given" << std::endl;
			else
				sendf(server, dest,  ERR_INVALIDKEY, str1);
			break ;
		case 691 :
			sendf(server, dest,  ERR_STARTTLS);
			break ;
		case 696 :
			if (!str1 || !str2 || !str3 || !str4)
				std::cout << m_errorCode << " not enough args were given" << std::endl;
			else
				sendf(server, dest,  ERR_INVALIDMODEPARAM, str1, str2, str3, str4);
			break ;
		case 723 :
			if (!str1)
				std::cout << m_errorCode << " no private command was given" << std::endl;
			else
				sendf(server, dest,  ERR_NOPRIVS, str1);
			break ;
		case 902 :
			sendf(server, dest,  ERR_NICKLOCKED);
			break ;
		case 904 :
			sendf(server, dest,  ERR_SASLFAIL);
			break ;
		case 905 :
			sendf(server, dest,  ERR_SASLTOOLONG);
			break ;
		case 906 :
			sendf(server, dest,  ERR_SASLABORTED);
			break ;
		case 907 :
			sendf(server, dest,  ERR_SASALREADY);
			break ;
		default :
			break;
	}
	va_end(args);
}