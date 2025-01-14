/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 13:03:21 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/14 13:38:09 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INVITE_HPP
# define INVITE_HPP
# include "Parsing.hpp"

class Server;
class Client;

class	Invite
{
	public :
		static void execute(Server &server, const Parsing &parse, Client &client);
};

#endif
