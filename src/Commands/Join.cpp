/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 17:19:53 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/12 16:14:49 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Join.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include <iostream>

void Join::execute(Server *server, const std::string buffer, Client &client)
{
	(void) server;
	(void) buffer;
	(void) client;
	std::cout << "coucou" << std::endl;
//	server->sendMsg(client, "Coucou frerot", "");

}
