/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 16:55:53 by noda              #+#    #+#             */
/*   Updated: 2025/01/08 18:50:13 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KICK_HPP
# define KICK_HPP
# define KICK ":%P KICK %C %T :%m"
class Server;
class Client;
class Parsing;

class Kick
{
    public :
	    static void execute(Server &server, const Parsing &parse, Client &client);
}   ;

#endif