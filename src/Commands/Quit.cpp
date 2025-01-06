/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 14:56:08 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/06 20:30:37 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Quit.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "utils.hpp"
#include "Parsing.hpp"
#include "serverExceptions.hpp"

void Quit::execute(Server &server, const Parsing &parse, Client &client)
{
	std::string	message;
	if (parse.getArguments().size() < 2)
		message = "";
	else
		message = parse.getArguments()[1];
	client.sendQuitMsg(&server, message);
	sendf(&server, &client, ":%h ERROR :Closing Link: %c (Quit: %m)", client.getNick().c_str(), message.c_str()); 
	std::cout << RED << "Client <" << client.getFD() << "> Disconnected" << WHI << std::endl;
	server.ClearClient(client);
}

