/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivMsg.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 15:31:04 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/13 16:28:13 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRIVMSG_HPP
# define PRIVMSG_HPP
# include "Parsing.hpp"
# define PRIVMSG ":%P PRIVMSG %n :%m" // source, nickname, message
# define PRIVMSGALL ":%P PRIVMSG %C :%m" // source, nickname, message

class Server;
class Client;

class	PrivMsg
{
	public :
		static void execute(Server &server, const Parsing &parse, Client &client);
};

#endif
