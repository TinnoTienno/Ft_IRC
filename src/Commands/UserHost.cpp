/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserHost.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 15:34:50 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/12 16:14:53 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UserHost.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include <iostream>

void UserHost::execute(Server *server, const std::string buffer, Client &client)
{
	(void) buffer;
	(void) server;
	(void) client;
//	server->sendMsg(client, "UserHost is" + server->getHostname(), "");
}
