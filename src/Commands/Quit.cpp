/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduvilla <aduvilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 14:56:08 by aduvilla          #+#    #+#             */
/*   Updated: 2024/12/24 15:57:17 by aduvilla         ###   ########.fr       */
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

void Quit::execute(Server *server, const Parsing &parse, Client &client)
{
	std::string	message;
	if (parse.getArguments().size() < 2)
		message = "";
	else
		message = parse.getArguments()[1];
	// chercher tous les chan sur lequel le client est présent
	// envoyer à tous les user de ces chan un message :
	// :clientnick!clientuser@host QUIT :parse.getArguments()[1]
	// sendf(server, usersurchan, ":%s QUIT :Quit: %m", client.getPrefix().c_str(), parse.getArguments()[1]);
	sendf(server, &client, ":%h ERROR :Closing Link: %c (Quit: %m)", client.getNick().c_str(), message.c_str()); 
	std::cout << RED << "Client <" << client.getFD() << "> Disconnected" << WHI << std::endl;
	server->ClearClient(client);
}

