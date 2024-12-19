/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivMsg.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduvilla <aduvilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 15:31:04 by aduvilla          #+#    #+#             */
/*   Updated: 2024/12/19 11:59:49 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRIVMSG_HPP
# define PRIVMSG_HPP
# include "Parsing.hpp"
# define PRIVMSG ":%s PRIVMSG %n :%m" // source, nickname, message

class Server;
class Client;

class	PrivMsg
{
	public :
		static void execute(Server *server, const Parsing &parse, Client &client);
};

#endif
