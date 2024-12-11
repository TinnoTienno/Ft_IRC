/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserHost.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 15:34:50 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/09 17:45:29 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UserHost.hpp"
#include <iostream>

void UserHost::execute(Server *server, const std::string buffer, Client &client)
{
	(void) buffer;
	server->sendMsg(client, "UserHost is" + server->getName(), "");
}