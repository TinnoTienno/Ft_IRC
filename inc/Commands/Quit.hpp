/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 14:54:11 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/10 18:32:01 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUIT_HPP
# define QUIT_HPP
#define QUITMSG ":%P QUIT :Quit: %m"

class Server;
class Client;
class Parsing;

class Quit
{
	public :
		static void execute(Server &server, const Parsing &parse, Client &client);
}	;

#endif
