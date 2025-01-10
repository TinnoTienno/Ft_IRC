/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduvilla <aduvilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 13:03:21 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/10 14:11:43 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INVITE_HPP
# define INVITE_HPP
# define INVITE ":%p INVITE %u %c" // guest chan
# include "Parsing.hpp"

class Server;
class Client;

class	Invite
{
	public :
		static void execute(Server &server, const Parsing &parse, Client &client);
};

#endif
