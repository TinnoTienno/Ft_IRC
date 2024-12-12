/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserHost.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 15:34:29 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/12 15:59:32 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USERHOST_HPP
#define USERHOST_HPP
# include <string>

class Server;
class Client;

class UserHost
{
	public :
		static void execute(Server *server, const std::string buffer, Client &client);
}	;

#endif