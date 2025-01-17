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

/**
 * @brief  Removes newlines and carriage returns from the given string.
 *
 * Trims leading colon and trailing newline or carriage return characters
 * from the input string.
 *
 * @param  str : The input string to be trimmed.
 *
 * @return A new string without leading or trailing newlines or carriage returns.
 */
std::string	trimNewLines(const std::string & str)
{
	std::string result = str;
	if (!result.empty() && result[0] == ':')
		result.erase(0, 1);
	while (!result.empty() && (result[result.size() - 1] == '\r' || result[result.size() - 1] == '\n'))
		result.erase(result.size() - 1, 1);
	return result;
}


/**
 * @brief  Sends a message from the bot to the server.
 *
 * Outputs the message to the console and sends it via the bot's socket.
 * Throws an exception if the message fails to send.
 *
 * @param  msg : The message to be sent.
 *
 * @throw std::runtime_error if the message is not sent successfully.
 */
void	Bot::speak(const std::string & msg)
{
	std::cout << "<< " << msg << std::endl;
	if (send(this->m_serSocket, msg.c_str(), msg.size(), 0) != static_cast<ssize_t>(msg.length()))
		throw std::runtime_error("Error: Message not sent");
}

/**
 * @brief  Disconnects the bot from the server.
 *
 * Closes the server socket if it is open and outputs a disconnection message.
 *
 * @return 1 to indicate successful disconnection.
 */
int	Bot::quit()
{
	if (this->m_serSocket != -1)
		close(this->m_serSocket);
	std::cout << "ircbot Disconnected" << std::endl;
	return 1;
}

/**
 * @brief  Splits a string into substrings based on a given delimiter.
 *
 * Splits the input string at each occurrence of the specified delimiter and
 * trims any leading or trailing newlines from the resulting substrings.
 *
 * @param  str : The string to be split.
 * @param  delimiter : The character used to split the string.
 *
 * @return A vector of substrings.
 */
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

/**
 * @brief  Retrieves the size of a file.
 *
 * Gets the size of the file in bytes based on the current cursor position
 * and resets the cursor to the beginning of the file.
 *
 * @param  file : The file stream whose size is to be retrieved.
 *
 * @return The size of the file in bytes.
 */
size_t	getFileSize(std::ifstream & file)
{
	size_t	result;
	result = file.tellg(); // get the file size in bytes with the current cursor position
	file.seekg(0, std::ios::beg); // move the cursor back to the begining
	return result;
}

/**
 * @brief  Converts a string to lowercase.
 *
 * @param  str : The string to be converted to lowercase.
 *
 * @return A new string with all characters in lowercase.
 */
std::string	toLowerStr(const std::string & str)
{
	std::string	result = str;
	for (std::string::iterator it = result.begin(); it != result.end(); ++it)
		*it = std::tolower(*it);
	return result;
}
