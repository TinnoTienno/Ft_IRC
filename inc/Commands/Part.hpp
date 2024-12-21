/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 14:17:03 by noda              #+#    #+#             */
/*   Updated: 2024/12/21 14:50:00 by noda             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PART_HPP
# define PART_HPP

class Server;
class Client;
class Parsing;

class Part
{
    public :
        static void execute(Server &server, const Parsing &parse, Client &client);
}   ;

#endif