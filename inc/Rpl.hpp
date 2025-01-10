/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Rpl.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 17:46:21 by eschussl          #+#    #+#             */
/*   Updated: 2025/01/10 12:06:41 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RPL_HPP
# define RPL_HPP

# define AUTH_IDENT ":%h NOTICE AUTH :*** No Identd"
# define AUTH_LOOKHOST ":%h NOTICE AUTH :*** Looking up your hostname"
# define AUTH_HOSTFOUND ":%h NOTICE AUTH :*** Found your hostname"
# define AUTH_HOSTNOTFOUND ":%h NOTICE AUTH :*** Hostname not found, IP instead"

# define RPL_WELCOME ":%h 001 %n :Welcome to the ft_IRC NETWORK %p"
# define RPL_YOURHOST ":%h 002 %n :Your host is %h, running version 1.2.3"
# define RPL_CREATED ":%h 003 %n :This server was created %T" // getTime
# define RPL_MYINFO ":%h 004 %n :%h 1.2.3 itkOl"
# define RPL_ISUPPORT ":%h 005 %n :CHANMODES=i, t, k, o, l : are supported by this server"
# define RPL_LUSERCLIENT ":%h 251 %n :There are %u users on 1 server" // getUserNumber
//# define RPL_LUSEROP ":%h 252 %n :0 :IRC Operators online"
# define RPL_LUSEROP ":%h 252 %n :Operators are disabled on this server"
# define RPL_LUSERCHANNELS ":%h 254 %n :%C :channels formed" // getChannelNumber
# define RPL_MOTDSTART ":%h 375 %n :- %h Message of the Day -"
# define RPL_MOTD ":%h 372 %n :%m" // message
# define RPL_ENDOFMOTD ":%h 376 %n :%h End of /MOTD command."
#define RPL_TOPIC "%h 332 %p %C :%t" // channel topic
#define RPL_NOTOPIC "%h 331 %p %C :No topic is set" // channel
#define RPL_NAMREPLY ":%p 353 %n%s %C :%l" //symbol channel list of clients
#define RPL_ENDOFNAMES ":%p 366 %n %C :End of /NAMES list." // nick channel
#endif
