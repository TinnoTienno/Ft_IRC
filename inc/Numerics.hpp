/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Numerics.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 13:41:39 by eschussl          #+#    #+#             */
/*   Updated: 2025/01/14 13:44:29 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NUMERICS_HPP
# define NUMERICS_HPP

// AUTHENTIFICATION

# define AUTH_IDENT ":%h NOTICE AUTH :*** No Identd"
# define AUTH_LOOKHOST ":%h NOTICE AUTH :*** Looking up your hostname"
# define AUTH_HOSTFOUND ":%h NOTICE AUTH :*** Found your hostname"
# define AUTH_HOSTNOTFOUND ":%h NOTICE AUTH :*** Hostname not found, IP instead"

//COMMANDS

# define INVITE ":%P INVITE %n %C" // guest chan
# define JOIN ":%P JOIN :%C"
# define KICK ":%P KICK %C %T :%m"
# define MODE ":%p MODE %C "
# define NICK ":%p NICK :%m"
# define NICK2 ":%m%p NICK :%m"
# define NOTICE ":%s NOTICE %n :%m" // source, nickname, message
# define PART ":%P PART %C :%m"
# define PONG ":%h PONG %c :%n" // getArguments()[1]
# define PRIVMSG ":%P PRIVMSG %n :%m" // source, nickname, message
# define PRIVMSGALL ":%P PRIVMSG %C :%m" // source, nickname, message
# define QUITMSG ":%P QUIT :Quit: %m"
# define TOPIC ":%p TOPIC %C :%T"

//RPL

# define RPL_WELCOME ":%h 001 %n :Welcome to the ft_IRC NETWORK %p"
# define RPL_YOURHOST ":%h 002 %n :Your host is %h, running version 1.2.3"
# define RPL_CREATED ":%h 003 %n :This server was created %T" // getTime
# define RPL_MYINFO ":%h 004 %n :%h 1.2.3 itkOl"
# define RPL_ISUPPORT ":%h 005 %n :CHANMODES=i, t, k, o, l : are supported by this server"
# define RPL_LUSERCLIENT ":%h 251 %n :There are %u users on 1 server" // getUserNumber
// # define RPL_LUSEROP ":%h 252 %n :0 :IRC Operators online"
# define RPL_LUSEROP ":%h 252 %n :Operators are disabled on this server"
# define RPL_LUSERCHANNELS ":%h 254 %n :%m :channels formed" // getChannelNumber
# define RPL_INVITELIST ":%h 336 %n %C"
# define RPL_ENDOFINVITELIST ":%h 337 %n :End of /INVITE list"
# define RPL_INVITING ":%h 341 %n %N %C" // guest channel
# define RPL_BANLIST ":%h 367 %n %C %m" // bannedPrefix
# define RPL_ENDOFBANLIST ":%h 368 %n %C :End of channel ban list"
# define RPL_MOTDSTART ":%h 375 %n :- %h Message of the Day -"
# define RPL_MOTD ":%h 372 %n :%m" // message
# define RPL_ENDOFMOTD ":%h 376 %n :%h End of /MOTD command."
#define RPL_TOPIC "%h 332 %p %C :%t" // channel topic
#define RPL_NOTOPIC ":%h 331 %n %C :No topic is set" // channel
#define RPL_NAMREPLY ":%h 353 %n%s %C :%l" //symbol channel list of clients
#define RPL_ENDOFNAMES ":%h 366 %n %C :End of /NAMES list." // nick channel
#define RPL_CHANNELMODEIS ":%h 324 %n %C %m" //

// Errors

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