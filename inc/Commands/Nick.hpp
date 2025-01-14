/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 17:10:50 by eschussl          #+#    #+#             */
/*   Updated: 2025/01/14 13:38:44 by eschussl         ###   ########.fr       */
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
		static void	execute(Server &server, const Parsing &parse, Client &client);
		static int parseError(Server &server, const Parsing &parse, Client &client);
		static bool	isNickFormatted(const std::string &nickname);
}	;

#endif