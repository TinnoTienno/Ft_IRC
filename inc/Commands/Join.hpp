/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 17:10:50 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/13 18:24:41 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JOIN_HPP
# define JOIN_HPP
# include <string>

class Server;
class Client;
class Parsing;

class Join
{
	public :
		static void execute(Server *server, const Parsing &parse, Client &client);
		static int errorCode(Server *server, const std::string &channel, Client &client);
		static int errorCode(Server *server, const std::string &channel, Client &client, const std::string passwd);

}	;

#endif