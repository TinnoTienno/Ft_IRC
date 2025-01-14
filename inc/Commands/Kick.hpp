/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 16:55:53 by noda              #+#    #+#             */
/*   Updated: 2025/01/14 13:38:30 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KICK_HPP
# define KICK_HPP
class Server;
class Client;
class Parsing;

class Kick
{
    public :
	    static void execute(Server &server, const Parsing &parse, Client &client);
}   ;

#endif