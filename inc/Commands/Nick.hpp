/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 17:10:50 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/12 15:59:20 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NICK_HPP
# define NICK_HPP
# include <string>

class Server;
class Client;

class Nick
{
	public :
		static void execute(Server *server, const std::string buffer, Client &client);
		static int checkformat(Server *server, const std::string &buffer);
}	;

#endif