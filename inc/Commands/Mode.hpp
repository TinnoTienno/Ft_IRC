/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 19:13:55 by noda              #+#    #+#             */
/*   Updated: 2025/01/15 13:19:46 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODE_HPP
# define MODE_HPP
# include <string>

class Server;
class Client;
class Parsing;
class Channel;
class Mode
{
	private :
		static void modeT(Server & server,Channel & channel, Client & source, bool status);
		static void modeI(Server & server, Channel & channel, Client & source, bool status);
		static void modeL(Server & server, Channel & channel, Client & source, bool status, const std::string & modeArg);
		static void modeK(Server & server, Channel & channel, Client & source, bool status, const std::string & modeArg);
		static void modeO(Server & server, Channel & channel, Client & source, bool status, const std::string & modeArg);
		static void	modeB(Server & server, Channel & channel, Client & source);
		static void	modeB(Server & server, Channel & channel, Client & source, bool status, const std::string bannedPrefix);
		static void channelMode(Server & server, Channel & channel, Client & source, const Parsing & parse);
    public :
		static void execute(Server & server, const Parsing & parse, Client & client);
}   ;


#endif
