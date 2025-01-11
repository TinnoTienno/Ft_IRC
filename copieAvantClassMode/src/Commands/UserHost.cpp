/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserHost.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 15:34:50 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/21 08:23:00 by noda             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UserHost.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include <iostream>
#include "Parsing.hpp"
#include "serverExceptions.hpp"

void UserHost::execute(Server &server, const Parsing &parse, Client &client)
{
	(void) server;
	(void) parse;
	(void) client;
//	server.sendMsg(client, "UserHost is" + server.getHostname(), "");
}
