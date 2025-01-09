/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 19:13:55 by noda              #+#    #+#             */
/*   Updated: 2025/01/09 16:15:39 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODE_HPP
# define MODE_HPP
#define MODE ":%p MODE %C "

class Server;
class Client;
class Parsing;
class Channel;
class Mode
{
	private :
		static void modeI(Channel &channel, bool status);
		static void channelMode(Server &server, Channel &channel, Client &source, const Parsing &parse);
    public :
		static void execute(Server &server, const Parsing &parse, Client &client);
}   ;


#endif