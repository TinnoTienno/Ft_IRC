/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Err.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduvilla <aduvilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 14:08:00 by eschussl          #+#    #+#             */
/*   Updated: 2025/01/10 15:40:45 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERR_HPP
# define ERR_HPP

#define ERR_NOSUCHNICK ":%h 401 %n %w :No such nick/channel" // wrongnick
#define ERR_NOSUCHCHANNEL ":%h 403 %n %c :No such channel" // channel
#define ERR_CANNOTSENDTOCHAN ":%h 404 %n %C :Cannot send to channel" // channel
#define ERR_TOOMANYCHANNELS ":%h 405 %n %C : You have joined too many channels" // channel
#define ERR_WASNOSUCHNICK ":%h 406 %n %w :There was no such nickname" // wrongnick
#define ERR_NOORIGIN ":%h 409 %s :No origin specified"
#define ERR_NORECIPIENT ":%h 411 %n :No recipient given (%c)" // command
#define ERR_NOTEXTTOSEND ":%h 412 %n :No text to send"
#define ERR_INPUTTOOLONG ":%h 417 %c :Input line was too long"
#define ERR_UNKNOWNCOMMAND ":%h 421 %n %c :Unknown command" // command
#define ERR_NOMOTD ":%h 422 %p :MOTD File is missing" //
#define ERR_NONICKNAMEGIVEN ":%h 431 %w :No nickname given" //
#define ERR_ERRONEUSNICKNAME ":%h 432 %w %n :Erroneus nickname" // wrongnickname
#define ERR_NICKNAMEINUSE ":%h 433 %n %w :Nickname is already in use" // wrongnickname
#define ERR_NICKCOLLISION ":%h 436 %n %w :Nickname collision KILL from %u@%H" // wrongnickname user clienthost
#define ERR_USERNOTINCHANNEL ":%h 441 %p %w %C :They aren't on that channel" // wrongnickname channel
#define ERR_NOTONCHANNEL ":%h 442 %n %C :You're not on that channel" // channel
#define ERR_USERONCHANNEL ":%h 443 %n %w %C :is already on channel" // wrongnickname channel
#define ERR_NOTREGISTERED ":%h 451 %c :You have not registered" // command
#define ERR_NEEDMOREPARAMS ":%h 461 %n %c :Not enough parameters" // command
#define ERR_ALREADYREGISTERED ":%h 462 %n :You may not reregister" //
#define ERR_PASSWDMISMATCH ":%h 464 %n :password incorrect" //
#define ERR_YOUREBANNEDCREEP ":%h 465 %n :You are banned from this server." //
#define ERR_CHANNELISFULL ":%h 471 %n %C :Cannot join channel (+l)" // channel
#define ERR_UNKNOWNMODE ":%h 472 %n %m :is unknown mode char to me" // modechararcter
#define ERR_INVITEONLYCHAN ":%h 473 %n %C :Cannot join channel (+i)" // nickname channel
#define ERR_BANNEDFROMCHAN ":%h 474 %n %C :Cannot join channel (+b)" // channel
#define ERR_BADCHANNELKEY ":%h 475 %n %C :Cannot join channel (+k)" // channel
#define ERR_BADCHANMASK ":%h 476 %C :Bad Channel Mask" // channel
#define ERR_NOPRIVILEGES ":%h 481 %n :Permission Denied- You're not an IRC operator" //
#define ERR_CHANOPRIVSNEEDED ":%h 482 %n %C :You're not channel operator" // channel
#define ERR_CANTKILLSERVER ":%h 483 %n :You cant kill a server!" //
#define ERR_NOOPERHOST ":%h 491 %n :No 0-lines for your" //
#define ERR_UMODEUNKNOWNFLAG ":%h 501 %n :Unknown MODE flag" //
#define ERR_USERDONTMATCH ":%h 502 %n :Cant change mode for other users" //
#define ERR_HELPNOTFOUND ":%h 524 %n %s :No help available on this topic" // subject (Can be lower case, if invalid parameter : *)
#define ERR_INVALIDKEY ":%h 525 %n %C :Key is not well-formed" // channel
#define ERR_STARTTLS ":%h 691 %n :STARTLS failed (wrong moon phase)" //
#define ERR_INVALIDMODEPARAM ":%h 696 %n %C %m %P :%D" // targetChannel modecharacter parameter description
#define ERR_NOPRIVS ":%h 723 %n %P :Insufficient oper privileges." // privateCommand(Defined by server)
#define ERR_NICKLOCKED "%h 902 %n :You must use a nick assigned to you" //
#define ERR_SASLFAIL "%h 904 %n :SASL authentification failed" //
#define ERR_SASLTOOLONG ":%h 905 %n :SASL message too long" //
#define ERR_SASLABORTED ":%h 906 %n :SASL authentification aborted" //
#define ERR_SASALREADY ":%h 907 %n :You have already authentificated using SASL" //

#endif
