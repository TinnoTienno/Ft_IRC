/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduvilla <aduvilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 14:11:32 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/15 23:54:57 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include <cstddef>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>
#include <ios>

std::string	trimNewLines(const std::string & str)
{
	std::string result = str;
	if (!result.empty() && result[0] == ':')
		result.erase(0, 1);
	while (!result.empty() && (result[result.size() - 1] == '\r' || result[result.size() - 1] == '\n'))
		result.erase(result.size() - 1, 1);
	return result;
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
		result.push_back(trimNewLines(token));	
	return result;
}

size_t	getFileSize(std::ifstream & file)
{
	size_t	result;
	result = file.tellg(); // get the file size in bytes with the current cursor position
	file.seekg(0, std::ios::beg); // move the cursor back to the begining
	return result;
}

std::string	toLowerStr(const std::string & str)
{
	std::string	result = str;
	for (std::string::iterator it = result.begin(); it != result.end(); ++it)
		*it = std::tolower(*it);
	return result;
}
