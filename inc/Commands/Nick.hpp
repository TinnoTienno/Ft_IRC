/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 17:10:50 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/13 15:21:16 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NICK_HPP
# define NICK_HPP
# include <string>

class Server;
class Client;
class Parsing;

class Nick
{
	public :
		static void execute(Server *server, const Parsing &parse, Client &client);
		static int checkformat(Server *server, const std::string &buffer);
		static int nickErrorCode(Server *server, Client &client, const Parsing &parse);

}	;

#endif