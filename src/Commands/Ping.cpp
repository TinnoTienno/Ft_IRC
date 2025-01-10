/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 15:59:44 by eschussl          #+#    #+#             */
/*   Updated: 2025/01/10 12:22:45 by aduvilla         ###   ########.fr       */
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
		sendf(&server, &client, ERR_NEEDMOREPARAMS, "PING");
	else if (parse.getArguments()[1].empty())		
		sendf(&server, &client, ERR_NOORIGIN);
	else
		sendf(&server, &client, PONG, parse.getArguments()[1].c_str());
}

