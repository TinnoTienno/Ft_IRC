/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserHost.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 15:34:50 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/11 17:26:33 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UserHost.hpp"
#include <iostream>

void UserHost::execute(Server *server, const std::string buffer, Client &client)
{
	(void) buffer;
	(void) server;
	(void) client;
//	server->sendMsg(client, "UserHost is" + server->getHostname(), "");
}
