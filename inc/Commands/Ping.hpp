/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ping.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 15:55:52 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/24 14:55:51 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PING_HPP
# define PING_HPP

class Server;
class Client;
class Parsing;

class Ping
{
	public :
		static void execute(Server *server, const Parsing &parse, Client &client);
}	;

#endif
