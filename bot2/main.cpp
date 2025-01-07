/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduvilla <aduvilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 10:39:46 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/06 12:57:22 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include <exception>
#include <iostream>
#include <cstdlib>
# define USAGE "Usage: ./bot [server adress] [name] [password] [port]"

int	main(int ac, char **av)
{
	if (ac != 5)
	{
		std::cout << USAGE << std::endl;
		return 1;
	}
	std::string port = av[4];
	if (port.find_first_not_of("0123456789") != std::string::npos)
	{
		std::cout << "Error: invalid argument: port: " + port << std::endl;
		return 1;
	}
	Bot	bot(av[1], av[2], av[3], std::atoi(av[4]));
//	if (!bot.init())
//		return 1
	try
	{
		if (!bot.init())
			return 1;
	} 
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;	
	}
	return 0;
}
