/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserHost.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 15:34:50 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/18 15:19:18 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UserHost.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include <iostream>
#include "Parsing.hpp"
#include "serverExceptions.hpp"

void UserHost::execute(Server *server, const Parsing &parse, Client &client)
{
	(void) server;
	(void) parse;
	(void) client;
//	server->sendMsg(client, "UserHost is" + server->getHostname(), "");
}
