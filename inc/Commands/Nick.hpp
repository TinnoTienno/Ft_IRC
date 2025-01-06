/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 17:10:50 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/21 08:22:24 by noda             ###   ########.fr       */
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