/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 17:10:50 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/17 17:51:55 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JOIN_HPP
# define JOIN_HPP
# include <string>
# include <exception>
# include <vector>

class Server;
class Client;
class Parsing;

class Join
{
	public :
		static void execute(Server *server, const Parsing &parse, Client &client);
		static void ifPassword(Server *server, const Parsing &parse, Client &client);
		static void  ifNoPassword(Server *server, const Parsing &parse, Client &client);
		static std::vector<std::string> parseChannels(const std::string &channelnames);
}	;

#endif