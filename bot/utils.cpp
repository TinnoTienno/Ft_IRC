/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduvilla <aduvilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 14:11:32 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/15 14:45:32 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

std::string	&Bot::m_trimNewLines(std::string & str)
{
	while (!str.empty() && (str[str.size() - 1] == '\r' || str[str.size() - 1] == '\n'))
		str.erase(str.size() - 1, 1);
	return str;
}

void	Bot::speak(const std::string & msg)
{
	std::cout << "<< " << msg << std::endl;
	if (send(this->m_serSocket, msg.c_str(), msg.size(), 0) != static_cast<ssize_t>(msg.length()))
		throw std::runtime_error("Error: Message not sent");
}

int	Bot::quit()
{
	if (this->m_serSocket != -1)
		close(this->m_serSocket);
	std::cout << "ircbot Disconnected" << std::endl;
	return 1;
}

std::vector<std::string>	vsplit(const std::string & str, char delimiter)
{
	std::vector<std::string>	result;
	std::string					token;
	if (str.empty())
		return result;
	std::istringstream	inputStream(str);
	while (std::getline(inputStream, token, delimiter))
		result.push_back(token);	
	return result;
}
