/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduvilla <aduvilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 15:59:44 by eschussl          #+#    #+#             */
/*   Updated: 2025/01/10 16:16:39 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Ping.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "utils.hpp"
#include "Parsing.hpp"
#include "Err.hpp"
#include "serverExceptions.hpp"

void Ping::execute(Server &server, const Parsing &parse, Client &client)
{
	if (parse.getArguments().size() == 1)
		server.sendf(&client, NULL, NULL, ERR_NEEDMOREPARAMS, "PING");
	else if (parse.getArguments()[1].empty())		
		server.sendf(&client, NULL, NULL, ERR_NOORIGIN);
	else
		server.sendf(&client, NULL, NULL, PONG, parse.getArguments()[1].c_str());
}

