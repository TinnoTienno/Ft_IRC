/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 17:05:11 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/18 15:19:49 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"
#include <exception>
#include <iostream>
#include <string>
#include <csignal>
#include "serverExceptions.hpp"

int main(int argc, char **argv)
{
	try
	{
		signal(SIGINT, Server::SignalHandler);
		signal(SIGINT, Server::SignalHandler);
		
		if (argc != 3)
			throw std::runtime_error("Error: format: ./ircserv port password");
		char hostname[256];
		gethostname(hostname, sizeof(hostname));
		Server server(hostname , argv[2]);
		server.ServerInit(argv[1]);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}
