/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 17:10:50 by eschussl          #+#    #+#             */
/*   Updated: 2025/01/14 13:38:24 by eschussl         ###   ########.fr       */
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
		static void execute(Server &server, const Parsing &parse, Client &client);
		static void execChannels(Server &server, const Parsing &parse, Client &client);
		static std::vector<std::string> parseChannels(const std::string &channelnames);
}	;

#endif
