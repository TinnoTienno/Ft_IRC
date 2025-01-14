/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivMsg.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 15:31:04 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/14 13:42:53 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRIVMSG_HPP
# define PRIVMSG_HPP
# include "Parsing.hpp"


class Server;
class Client;

class	PrivMsg
{
	public :
		static void execute(Server &server, const Parsing &parse, Client &client);
};

#endif
