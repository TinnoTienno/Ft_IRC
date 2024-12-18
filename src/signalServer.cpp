/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signalServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduvilla <aduvilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 14:00:27 by aduvilla          #+#    #+#             */
/*   Updated: 2024/12/17 14:16:36 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>

void Server::SignalHandler(int signum)
{
	(void)signum;
	std::cout << std::endl << "Signal Received!" << std::endl;
	Server::m_signal = true; //-> set the static boolean to true to stop the server
}