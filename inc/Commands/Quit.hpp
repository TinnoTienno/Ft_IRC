/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 14:54:11 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/09 16:44:36 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUIT_HPP
# define QUIT_HPP
#define QUIT ":%N ERROR :Closing Link: %C (Quit: %m)"

class Server;
class Client;
class Parsing;

class Quit
{
	public :
		static void execute(Server &server, const Parsing &parse, Client &client);
}	;

#endif
