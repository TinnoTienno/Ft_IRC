/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 15:31:20 by noda              #+#    #+#             */
/*   Updated: 2024/12/21 15:32:20 by noda             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOPIC_HPP
# define TOPIC_HPP

class Server;
class Client;
class Parsing;

class Topic
{
    public :
		static void	execute(Server &server, const Parsing &parse, Client &client);
}   ;
#endif