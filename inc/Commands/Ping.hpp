/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ping.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 15:55:52 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/21 08:22:24 by noda             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PING_HPP
# define PING_HPP
# include <string>

class Server;
class Client;
class Parsing;

class Ping
{
	public :
		static void execute(Server &server, const Parsing &parse, Client &client);
}	;

#endif