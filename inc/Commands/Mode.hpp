/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 19:13:55 by noda              #+#    #+#             */
/*   Updated: 2025/01/08 18:55:12 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODE_HPP
# define MODE_HPP
#define MODE ":%p MODE %c "

class Server;
class Client;
class Parsing;
class Mode
{
    public :
		static void execute(Server &server, const Parsing &parse, Client &client);
}   ;


#endif