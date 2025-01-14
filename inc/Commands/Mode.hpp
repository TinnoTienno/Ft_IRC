/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 19:13:55 by noda              #+#    #+#             */
/*   Updated: 2025/01/14 13:43:37 by eschussl         ###   ########.fr       */
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
		static void modeT(Channel & channel, bool status);
		static void modeI(Channel &channel, bool status);
		static void modeL(Channel & channel, bool status, const std::string &modeArg);
		static void modeK(Channel & channel, bool status, const std::string & modeArg);
		static void modeO(Server &server, Channel & channel, Client &source, bool status, const std::string & modeArg);
		static void	modeB(Channel &channel, Client &source);
		static void	modeB(Channel &channel, bool status, const std::string bannedPrefix);
		static void channelMode(Server &server, Channel &channel, Client &source, const Parsing &parse);
    public :
		static void execute(Server &server, const Parsing &parse, Client &client);
}   ;


#endif
