/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivMsg.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduvilla <aduvilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 15:31:04 by aduvilla          #+#    #+#             */
/*   Updated: 2024/12/16 19:48:07 by aduvilla         ###   ########.fr       */
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
		static void execute(Server *server, Client &client, const Parsing &parse);
};

#endif
