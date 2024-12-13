/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 17:10:50 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/13 13:59:21 by eschussl         ###   ########.fr       */
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
}	;

#endif