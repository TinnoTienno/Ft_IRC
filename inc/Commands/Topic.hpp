/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 15:31:20 by noda              #+#    #+#             */
/*   Updated: 2025/01/08 18:58:28 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOPIC_HPP
# define TOPIC_HPP
# define TOPIC ":%p TOPIC %C :%T"
class Server;
class Client;
class Parsing;

class Topic
{
    public :
		static void	execute(Server &server, const Parsing &parse, Client &client);
}   ;
#endif