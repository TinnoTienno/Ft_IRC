/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Notice.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 23:57:08 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/06 13:57:30 by noda             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NOTICE_HPP
# define NOTICE_HPP
# include "Parsing.hpp"
# define NOTICE ":%s NOTICE %n :%m" // source, nickname, message

class Server;
class Client;

class	Notice
{
	public :
		static void execute(Server &server, const Parsing &parse, Client &client);
};

#endif
