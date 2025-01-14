/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Notice.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 23:57:08 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/14 13:42:34 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NOTICE_HPP
# define NOTICE_HPP
# include "Parsing.hpp"

class Server;
class Client;

class	Notice
{
	public :
		static void execute(Server &server, const Parsing &parse, Client &client);
};

#endif
