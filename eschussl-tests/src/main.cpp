/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 17:05:11 by eschussl          #+#    #+#             */
/*   Updated: 2024/11/22 17:34:02 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"
#include <exception>
#include <iostream>
#include <string>

int main(int argc, char **argv)
{
	try
	{
		if (argc != 3)
		throw std::runtime_error("Error: format: ./ircserv port password");
		Server server;
		server.ServerInit(argv[1]);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}