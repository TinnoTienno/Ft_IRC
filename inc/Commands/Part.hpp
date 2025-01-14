/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 14:17:03 by noda              #+#    #+#             */
/*   Updated: 2025/01/14 13:42:41 by eschussl         ###   ########.fr       */
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