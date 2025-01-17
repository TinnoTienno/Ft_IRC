/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduvilla <aduvilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 17:43:19 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/14 15:03:09 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unistd.h>
#include <vector>
#include <ctime>
#include <netdb.h>
#include <vector>
#include <sstream>
#include <stdio.h>
#include <limits.h>

/**
 * @brief Converts an integer to a string.
 * 
 * @param nb The integer to convert.
 * @return std::string The resulting string.
 */
std::string itoa(int nb)
{
	std::ostringstream oss;
	oss << nb;
	return (std::string) oss.str();
}

/**
 * @brief Gets the current time as a formatted string.
 * 
 * @return const std::string The formatted time string.
 */
const std::string	getTime()
{
	char				buffer[100];	
	std::time_t	now =std::time(0);
	std::tm	*timeStruct = std::gmtime(&now);
	std::strftime(buffer, sizeof(buffer), "%a %b %d %Y at %H:%M:%S GMT", timeStruct);
	return std::string(buffer);
}

/**
 * @brief Splits a string into a vector of substrings based on a delimiter.
 * 
 * @param str The string to split.
 * @param delimiter The character to use as the delimiter.
 * @return std::vector<std::string> The resulting vector of substrings.
 */
std::vector<std::string>	vsplit(const std::string & str, char delimiter)
{
	std::vector<std::string>	result;
	std::string					token;
	if (str.empty())
		return result;
	std::stringstream			strStream(str);

	while (std::getline(strStream, token, delimiter))
		result.push_back(token);
	return result;
}

/**
 * @brief Converts a string to an unsigned short.
 * 
 * @param str The string to convert.
 * @return unsigned short The resulting unsigned short.
 * @throws std::invalid_argument if the string is empty or contains non-digit characters.
 */
unsigned short	strtous(const std::string & str)
{
	unsigned short	result = 0;
	unsigned long	longVal;

	if (str.empty())
		throw std::invalid_argument("Error: Empty string");
	if (str.find_first_not_of("0123456789") != std::string::npos)
		throw std::invalid_argument("Error: Invalid argument: Only digit accepted: " + str);
	longVal = strtoul(str.c_str(), NULL, 10);
	if (longVal > USHRT_MAX)
		throw std::invalid_argument("Error: Invalid argument: Too long; " + str);
	result = static_cast<unsigned short>(longVal);
	return result;
}

/**
 * @brief Checks if a character is not in a given string.
 * 
 * @param c The character to check.
 * @param plage The string to check against.
 * @return true If the character is not in the string.
 * @return false If the character is in the string.
 */
bool charIsNot(char c, const std::string &plage)
{
	for (std::string::const_iterator iter = plage.begin(); iter != plage.end(); iter++)
		if (c == *iter)
			return false;
	return true;
}

/**
 * @brief Compares two strings case-insensitively.
 * 
 * @param string1 The first string.
 * @param string2 The second string.
 * @return true If the strings are equal (case-insensitive).
 * @return false If the strings are not equal.
 */
bool strCompareNoCase(const std::string &string1, const std::string &string2)
{
	if (string1.size() != string2.size())
		return false;
	for (size_t i = 0; i < string1.size(); i++)
		if (tolower(string1[i]) != tolower(string2[i]))
			return false;
	return true;
}

/**
 * @brief Trims newline characters from the end of a string.
 * 
 * @param str The string to trim.
 * @return std::string The resulting string.
 */
std::string	trimNewLines(const std::string & str)
{
	std::string	result = str;
	while (!str.empty() && (str[str.size() - 1] == '\r' || str[str.size() - 1] == '\n'))
		result.erase(str.size() - 1, 1);
	return result;
}
