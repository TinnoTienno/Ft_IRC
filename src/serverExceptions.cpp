/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverExceptions.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 16:06:42 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/18 17:14:16 by eschussl         ###   ########.fr       */
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
	if (!server || dest)
		return ;
	va_list args;
	va_start(args, dest);
	
	char *str1 = va_arg(args, char *);
	char *str2 = va_arg(args, char *);
	char *str3 = va_arg(args, char *);
	char *str4 = va_arg(args, char *);
	switch(m_errorCode)
	{
		case 401 :
			if (!str1)
				std::cout << m_errorCode << " no nickname was given" << std::endl;
			else
				sendf(server, dest, (std::string) ERR_NOSUCHNICK, str1);
			break ;
		case 403 :
			if (!str1)
				std::cout << m_errorCode << " no channel was given" << std::endl;
			else
				sendf(server, dest, (std::string) ERR_NOSUCHCHANNEL, str1);
			break ;
		case 404 :
			if (!str1)
				std::cout << m_errorCode << " no channel was given" << std::endl;
			else
				sendf(server, dest, (std::string) ERR_CANNOTSENDTOCHAN, str1);
			break ;
		case 405 :
			if (!str1)
				std::cout << m_errorCode << " no channel was given" << std::endl;
			else
				sendf(server, dest, (std::string) ERR_TOOMANYCHANNELS, str1);
			break ;
		case 406 :
			if (!str1)
				std::cout << m_errorCode << " no nickname was given" << std::endl;
			else
				sendf(server, dest, (std::string) ERR_WASNOSUCHNICK, str1);
			break ;
		case 409 :
			sendf(server, dest, (std::string) ERR_NOORIGIN);
			break ;
		case 411 :
			if (!str1)
				std::cout << m_errorCode << " no command was given" << std::endl;
			else
				sendf(server, dest, (std::string) ERR_NORECIPIENT, str1);
			break ;	
		case 412 :
			sendf(server, dest, (std::string) ERR_NOTEXTTOSEND);
			break ;
		case 417 :
			sendf(server, dest, (std::string) ERR_INPUTTOOLONG);
			break ;	
		case 421 :
			if (!str1)
				std::cout << m_errorCode << " no command was given" << std::endl;
			else
				sendf(server, dest, (std::string) ERR_UNKNOWNCOMMAND, str1);
			break ;	
		case 422 :
			sendf(server, dest, (std::string) ERR_NOMOTD);
			break ;
		case 431 :
			sendf(server, dest, (std::string) ERR_NONICKNAMEGIVEN);
			break ;
		case 432 :
			if (!str1)
				std::cout << m_errorCode << " no nickname was given" << std::endl;
			else
				sendf(server, dest, (std::string) ERR_ERRONEUSNICKNAME, str1);
			break ;	
		case 433 :
			if (!str1)
				std::cout << m_errorCode << " no nickname was given" << std::endl;
			else
				sendf(server, dest, (std::string) ERR_NICKNAMEINUSE, str1);
			break ;
		case 436 :
			if (!str1 || !str2 || !str3)
				std::cout << m_errorCode << " not enough args were given" << std::endl;
			else
				sendf(server, dest, (std::string) ERR_NICKCOLLISION, str1, str2, str3);
			break ;	
		case 441 :
			if (!str1 || !str2)
				std::cout << m_errorCode << " not enough args were given" << std::endl;
			else
				sendf(server, dest, (std::string) ERR_USERNOTINCHANNEL, str1, str2);
			break ;
		case 442 :
			if (!str1)
				std::cout << m_errorCode << " no channel was given" << std::endl;
			else
				sendf(server, dest, (std::string) ERR_NOTONCHANNEL, str1);
			break ;
		case 443 :
			if (!str1 || !str2)
				std::cout << m_errorCode << " not enough args were given" << std::endl;
			else
				sendf(server, dest, (std::string) ERR_USERONCHANNEL, str1, str2);
			break ;
		case 451 :
			sendf(server, dest, (std::string) ERR_NOTREGISTERED);
			break ;
		case 461 :
			if (!str1)
				std::cout << m_errorCode << " no command was given" << std::endl;
			else
				sendf(server, dest, (std::string) ERR_NEEDMOREPARAMS, str1);
			break ;
		case 462 :
			sendf(server, dest, (std::string) ERR_ALREADYREGISTERED);
			break ;
		case 464 :
			sendf(server, dest, (std::string) ERR_PASSWDMISMATCH);
			break ;
		case 465 :
			sendf(server, dest, (std::string) ERR_YOUREBANNEDCREEP);
			break ;
		case 471 :
			if (!str1)
				std::cout << m_errorCode << " no channel was given" << std::endl;
			else
				sendf(server, dest, (std::string) ERR_CHANNELISFULL, str1);
			break ;
		case 472 :
			if (!str1)
				std::cout << m_errorCode << " no modechararcter was given" << std::endl;
			else
				sendf(server, dest, (std::string) ERR_UNKNOWNMODE, str1);
			break ;
		case 473 :
			if (!str1)
				std::cout << m_errorCode << " no channel was given" << std::endl;
			else
				sendf(server, dest, (std::string) ERR_INVITEONLYCHAN, str1);
			break ;
		case 474 :
			if (!str1)
				std::cout << m_errorCode << " no channel was given" << std::endl;
			else
				sendf(server, dest, (std::string) ERR_BANNEDFROMCHAN, str1);
			break ;
		case 475 :
			if (!str1)
				std::cout << m_errorCode << " no channel was given" << std::endl;
			else
				sendf(server, dest, (std::string) ERR_BADCHANNELKEY, str1);
			break ;
		case 476 :
			if (!str1)
				std::cout << m_errorCode << " no channel was given" << std::endl;
			else
				sendf(server, dest, (std::string) ERR_BADCHANMASK, str1);
			break ;
		case 481 :
			sendf(server, dest, (std::string) ERR_NOPRIVILEGES);
			break ;
		case 482 :
			if (!str1)
				std::cout << m_errorCode << " no channel was given" << std::endl;
			else
				sendf(server, dest, (std::string) ERR_CHANOPRIVSNEEDED, str1);
			break ;
		case 483 :
			sendf(server, dest, (std::string) ERR_CANTKILLSERVER);
			break ;
		case 491 :
			sendf(server, dest, (std::string) ERR_NOOPERHOST);
			break ;
		case 501 :
			sendf(server, dest, (std::string) ERR_UMODEUNKNOWNFLAG);
			break ;
		case 502 :
			sendf(server, dest, (std::string) ERR_USERDONTMATCH);
			break ;
		case 524 :
			if (!str1)
				std::cout << m_errorCode << " no subject was given" << std::endl;
			else
				sendf(server, dest, (std::string) ERR_HELPNOTFOUND, str1);
			break ;
		case 525 :
			if (!str1)
				std::cout << m_errorCode << " no channel was given" << std::endl;
			else
				sendf(server, dest, (std::string) ERR_INVALIDKEY, str1);
			break ;
		case 691 :
			sendf(server, dest, (std::string) ERR_STARTTLS);
			break ;
		case 696 :
			if (!str1 || !str2 || !str3 || !str4)
				std::cout << m_errorCode << " not enough args were given" << std::endl;
			else
				sendf(server, dest, (std::string) ERR_INVALIDMODEPARAM, str1, str2, str3, str4);
			break ;
		case 723 :
			if (!str1)
				std::cout << m_errorCode << " no private command was given" << std::endl;
			else
				sendf(server, dest, (std::string) ERR_NOPRIVS, str1);
			break ;
		case 902 :
			sendf(server, dest, (std::string) ERR_NICKLOCKED);
			break ;
		case 904 :
			sendf(server, dest, (std::string) ERR_SASLFAIL);
			break ;
		case 905 :
			sendf(server, dest, (std::string) ERR_SASLTOOLONG);
			break ;
		case 906 :
			sendf(server, dest, (std::string) ERR_SASLABORTED);
			break ;
		case 907 :
			sendf(server, dest, (std::string) ERR_SASALREADY);
			break ;
		default :
			break;
	}
}