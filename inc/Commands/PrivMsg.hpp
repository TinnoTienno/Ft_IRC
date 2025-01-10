/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivMsg.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 15:31:04 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/08 18:42:52 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRIVMSG_HPP
# define PRIVMSG_HPP
# include "Parsing.hpp"
# define PRIVMSG ":%P PRIVMSG %n :%m" // source, nickname, message

class Server;
class Client;

class	PrivMsg
{
	public :
		static void execute(Server &server, const Parsing &parse, Client &client);
};

#endif
