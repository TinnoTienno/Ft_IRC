/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 17:10:50 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/12 15:59:22 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JOIN_HPP
# define JOIN_HPP
# include <string>

class Server;
class Client;

class Join
{
	public :
		static void execute(Server *server, const std::string buffer, Client &client);
}	;

#endif