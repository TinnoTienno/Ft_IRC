/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ping.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 15:55:52 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/12 16:14:11 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PING_HPP
# define PING_HPP
# include <string>

class Server;
class Client;

class Ping
{
	public :
		static void execute(Server *server, const std::string buffer, Client &client);
}	;

#endif