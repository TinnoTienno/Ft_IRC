/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 17:05:11 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/06 13:23:22 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"
#include <exception>
#include <iostream>
#include <string>
#include <csignal>

int main(int argc, char **argv)
{
	try
	{
		signal(SIGINT, Server::SignalHandler);
		signal(SIGINT, Server::SignalHandler);
		
		if (argc != 3)
			throw std::runtime_error("Error: format: ./ircserv port password");
		Server server(argv[2]);
		server.ServerInit(argv[1]);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}