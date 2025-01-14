/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 14:54:11 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/14 13:42:58 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUIT_HPP
# define QUIT_HPP

class Server;
class Client;
class Parsing;

class Quit
{
	public :
		static void execute(Server &server, const Parsing &parse, Client &client);
}	;

#endif
