/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduvilla <aduvilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 17:43:19 by aduvilla          #+#    #+#             */
/*   Updated: 2024/12/11 18:09:08 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <netdb.h>

void	sendMessage(const int fd, const std::string & source, const std::string & command, const std::string msg)
{
	std::string message = ":" + source + ".com " + command + " :" + msg + "\r\n";
	send(fd, message.c_str(), message.size(), 0);
	std::cout << message << std::endl;
}
