/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 19:13:55 by noda              #+#    #+#             */
/*   Updated: 2024/12/21 19:15:05 by noda             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODE_HPP
# define MODE_HPP

class Server;
class Client;
class Parsing;

class Mode
{
    public :
		static void execute(Server &server, const Parsing &parse, Client &client);
}   ;


#endif