/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 19:13:55 by noda              #+#    #+#             */
/*   Updated: 2025/01/13 10:31:32 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODE_HPP
# define MODE_HPP
#include <string>
#define MODE ":%p MODE %C "

class Server;
class Client;
class Parsing;
class Channel;
class Mode
{
	private :
		static void modeT(Channel & channel, bool status);
		static void modeI(Channel &channel, bool status);
		static void modeL(Channel & channel, bool status, const Parsing & parse);
		static void modeK(Channel & channel, bool status, const Parsing & parse);
		static void modeO(Channel & channel, bool status, const std::string & modeArg);
		static void channelMode(Server &server, Channel &channel, Client &source, const Parsing &parse);
    public :
		static void execute(Server &server, const Parsing &parse, Client &client);
}   ;


#endif
