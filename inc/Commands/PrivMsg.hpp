/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivMsg.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduvilla <aduvilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 15:31:04 by aduvilla          #+#    #+#             */
/*   Updated: 2024/12/13 15:33:32 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRIVMSG_HPP
# define PRIVMSG_HPP
# include <string>

class Server;
class Client;

class	PrivMsg
{
	public :
		static void execute(Server *server, const std::string buffer, Client &client);
		static int checkformat(Server *server, const std::string &buffer);
};

#endif
