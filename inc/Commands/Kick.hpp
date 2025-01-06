/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 16:55:53 by noda              #+#    #+#             */
/*   Updated: 2024/12/21 16:59:41 by noda             ###   ########.fr       */
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