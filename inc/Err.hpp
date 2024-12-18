/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Err.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 14:08:00 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/18 16:41:56 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERR_HPP
# define ERR_HPP

#define ERR_NOSUCHNICK "%h 401 %p %n :No such nick/channel" // nick
#define ERR_NOSUCHCHANNEL "%h 403 %p %C :No such channel" // channel
#define ERR_CANNOTSENDTOCHAN "%h 404 %p %C :Cannot send to channel" // channel
#define ERR_TOOMANYCHANNELS "%h 405 %p %C : You have joined too many channels" // channel
#define ERR_WASNOSUCHNICK "%h 406 %p %n :There was no such nickname" // nickname
#define ERR_NOORIGIN "%h 409 %p :No origin specified" //
#define ERR_NORECIPIENT "%h 411 %p :No recipient given (%c)" // command
#define ERR_NOTEXTTOSEND "%h 412 %p :No text to send" //
#define ERR_INPUTTOOLONG "%h 417 %c :Input line was too long" //
#define ERR_UNKNOWNCOMMAND "%h 421 %p %c :Unknown command" // command
#define ERR_NOMOTD "%h 422 %p :MOTD File is missing" //
#define ERR_NONICKNAMEGIVEN "%h 431 %p :No nickname given" //
#define ERR_ERRONEUSNICKNAME "%h 432 %p %n :Erroneus nickname" // nickname
#define ERR_NICKNAMEINUSE "%h 433 %p %n :Nickname is already in use" // nickname
#define ERR_NICKCOLLISION "%h 436 %p %n :Nickname collision KILL from %u@%H" // nickname user clienthost
#define ERR_USERNOTINCHANNEL "%h 441 %p %n %C :They aren't on that channel" // nickname channel
#define ERR_NOTONCHANNEL "%h 442 %p %C :You're not on that channel" // channel
#define ERR_USERONCHANNEL "%h 443 %p %n %C :is already on channel" // nickname channel
#define ERR_NOTREGISTERED "%h 451 %p :You have not registered" //
#define ERR_NEEDMOREPARAMS "%h 461 %p %c :Not enough parameters" // command
#define ERR_ALREADYREGISTERED "%h 462 %p :You may not reregister" //
#define ERR_PASSWDMISMATCH "%h 464 %p :password incorrect" //
#define ERR_YOUREBANNEDCREEP "%h 465 %p :You are banned from this server." //
#define ERR_CHANNELISFULL "%h 471 %p %C :Cannot join channel (+l)" // channel
#define ERR_UNKNOWNMODE "%h 472 %p %m :is unknown mode char to me" // modechararcter
#define ERR_INVITEONLYCHAN "%h 473 %p %C :Cannot join channel (+i)" // channel
#define ERR_BANNEDFROMCHAN "%h 474 %p %C :Cannot join channel (+b)" // channel
#define ERR_BADCHANNELKEY "%h 475 %p %C :Cannot join channel (+k)" // channel
#define ERR_BADCHANMASK "%h 476 %C :Bad Channel Mask" // channel
#define ERR_NOPRIVILEGES "%h 481 %p :Permission Denied- You're not an IRC operator" //
#define ERR_CHANOPRIVSNEEDED "%h 482 %p %C :You're not channel operator" // channel
#define ERR_CANTKILLSERVER "%h 483 %p :You cant kill a server!" //
#define ERR_NOOPERHOST "%h 491 %p :No 0-lines for your" //
#define ERR_UMODEUNKNOWNFLAG "%h 501 %p :Unknown MODE flag" //
#define ERR_USERDONTMATCH "%h 502 %p :Cant change mode for other users" //
#define ERR_HELPNOTFOUND "%h 524 %p %s :No help available on this topic" // subject (Can be lower case, if invalid parameter : *)
#define ERR_INVALIDKEY "%h 525 %p %C :Key is not well-formed" // channel
#define ERR_STARTTLS "%h 691 %p :STARTLS failed (wrong moon phase)" //
#define ERR_INVALIDMODEPARAM "%h 696 %p %C %m %P :%D" // targetChannel modecharacter parameter description
#define ERR_NOPRIVS "%h 723 %p %P :Insufficient oper privileges." // privateCommand(Defined by server)
#define ERR_NICKLOCKED "%h 902 %p :You must use a nick assigned to you" //
#define ERR_SASLFAIL "%h 904 %p :SASL authentification failed" //
#define ERR_SASLTOOLONG "%h 905 %p :SASL message too long" //
#define ERR_SASLABORTED "%h 906 %p :SASL authentification aborted" //
#define ERR_SASALREADY "%h 907 %p :You have already authentificated using SASL" //

#endif