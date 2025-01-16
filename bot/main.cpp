/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduvilla <aduvilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 10:39:46 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/16 10:37:43 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include <exception>
#include <iostream>
#include <cstdlib>
#include <csignal>
#include <string>
#ifndef BOT_USAGE
# define BOT_USAGE "Usage: ./bot [server adress] [Channel] [password] [port]"
#endif

void	Bot::signalHandler(int signum)
{
	std::cout << std::endl << "signal Received: " << signum << std::endl;
	Bot::m_signal = true;
}

int	main(int ac, char **av)
{
	if (ac != 5)
	{
		std::cout << BOT_USAGE << std::endl;
		return 1;
	}
	std::string port(av[4]);
	if (port.find_first_not_of("0123456789") != std::string::npos)
	{
		std::cout << "Error: invalid argument: port: " + port << std::endl;
		return 1;
	}
	std::string channel(av[2]);
	if (channel.empty())
	{
		std::cout << "Error: No channel name" << std::endl;
		return 1;
	}
	std::string adress(av[1]);
	if (!adress.empty() && adress == "localhost")
		adress = "127.0.0.1";
	Bot	bot(adress, channel, av[3], std::atoi(av[4]));
	try
	{
		signal(SIGINT, Bot::signalHandler);
		signal(SIGQUIT, Bot::signalHandler);
		signal(SIGPIPE, SIG_IGN);
		if (!bot.init())
			return 1;
	} 
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;	
	}
	return 0;
}
