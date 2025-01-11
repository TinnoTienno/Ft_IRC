/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserHost.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 15:34:29 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/21 08:22:24 by noda             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USERHOST_HPP
#define USERHOST_HPP
# include <string>

class Server;
class Client;
class Parsing;

class UserHost
{
	public :
		static void execute(Server &server, const Parsing &parse, Client &client);
}	;

#endif