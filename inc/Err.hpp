/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Err.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 14:08:00 by eschussl          #+#    #+#             */
/*   Updated: 2025/01/13 17:11:03 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERR_HPP
# define ERR_HPP

#define ERR_NOSUCHNICK ":%h 401 %n %m :No such nick" // wrongnick
#define ERR_NOSUCHCHANNEL ":%h 403 %n %m :No such channel" // wrongchannel
#define ERR_CANNOTSENDTOCHAN ":%h 404 %n %C :Cannot send to channel"
#define ERR_TOOMANYCHANNELS ":%h 405 %n %C : You have joined too many channels" 
#define ERR_WASNOSUCHNICK ":%h 406 %n %m :There was no such nickname" // wrongnick
#define ERR_NOORIGIN ":%h 409 %n :No origin specified"
#define ERR_NORECIPIENT ":%h 411 %n :No recipient given %m" // command
#define ERR_NOTEXTTOSEND ":%h 412 %n :No text to send"
#define ERR_INPUTTOOLONG ":%h 417 %n :Input line was too long"
#define ERR_UNKNOWNCOMMAND ":%h 421 %n %m :Unknown command" // command
#define ERR_NOMOTD ":%h 422 %n :MOTD File is missing" //
#define ERR_NONICKNAMEGIVEN ":%h 431 %n :No nickname given" //
#define ERR_ERRONEUSNICKNAME ":%h 432 %n %m :Erroneus nickname" // wrongnickname
#define ERR_NICKNAMEINUSE ":%h 433 %n %m :Nickname is already in use" // wrongnickname
#define ERR_NICKCOLLISION ":%h 436 %n %m :Nickname collision KILL from %u@%H" // wrongnickname user clienthost
#define ERR_USERNOTINCHANNEL ":%h 441 %n %m %C :They aren't on that channel" // wrongnickname
#define ERR_NOTONCHANNEL ":%h 442 %n %C :You're not on that channel" 
#define ERR_USERONCHANNEL ":%h 443 %n %m %C :is already on channel" // wrongnickname channel
#define ERR_NOTREGISTERED ":%h 451 %c :You have not registered" // client
#define ERR_NEEDMOREPARAMS ":%h 461 %n %m :Not enough parameters" // command
#define ERR_ALREADYREGISTERED ":%h 462 %n :You may not reregister" //
#define ERR_PASSWDMISMATCH ":%h 464 %n :password incorrect" //
#define ERR_YOUREBANNEDCREEP ":%h 465 %n :You are banned from this server." //
#define ERR_CHANNELISFULL ":%h 471 %n %C :Cannot join channel (+l)" // channel
#define ERR_UNKNOWNMODE ":%h 472 %n %m :is unknown mode char to me" // modecharacter
#define ERR_INVITEONLYCHAN ":%h 473 %n %C :Cannot join channel (+i)"
#define ERR_BANNEDFROMCHAN ":%h 474 %n %C :Cannot join channel (+b)" // channel
#define ERR_BADCHANNELKEY ":%h 475 %n %C :Cannot join channel (+k)" // channel
#define ERR_BADCHANMASK ":%h 476 %w :Bad Channel Mask" // wrongchannel
#define ERR_NOPRIVILEGES ":%h 481 %n :Permission Denied- You're not an IRC operator" 
#define ERR_CHANOPRIVSNEEDED ":%h 482 %n %C :You're not channel operator" // channel
#define ERR_CANTKILLSERVER ":%h 483 %n :You cant kill a server!" //
#define ERR_NOOPERHOST ":%h 491 %n :No 0-lines for your host	" //
#define ERR_UMODEUNKNOWNFLAG ":%h 501 %n :Unknown MODE flag" //
#define ERR_USERDONTMATCH ":%h 502 %n :Cant change mode for other users" //
#define ERR_HELPNOTFOUND ":%h 524 %n %m :No help available on this topic" // subject (Can be lower case, if invalid parameter : *)
#define ERR_INVALIDKEY ":%h 525 %n %w :Key is not well-formed" // wrongchannel
#define ERR_INVALIDMODEPARAM ":%h 696 %n %C %m %m :%m" // targetChannel modecharacter parameter description

#endif
