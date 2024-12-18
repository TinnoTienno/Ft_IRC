/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 17:43:19 by aduvilla          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/12/17 17:37:28 by eschussl         ###   ########.fr       */
=======
/*   Updated: 2024/12/17 15:37:32 by aduvilla         ###   ########.fr       */
>>>>>>> 2aef5294e1965bb4b2a8ba74b2a0886033821d3f
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>
#include <ctime>
#include <netdb.h>
#include <vector>
#include <sstream>


void	sendMessage(const int fd, const std::string & source, const std::string & command, const std::string msg)
{
	std::string message = ":" + source + " " + command + " :" + msg + "\r\n";
	send(fd, message.c_str(), message.size(), 0);
	std::cout << " " << fd << " << " << message << std::endl;
}

const std::string	getTime()
{
	char				buffer[100];	
	std::time_t	now =std::time(0);
	std::tm	*timeStruct = std::gmtime(&now);
	std::strftime(buffer, sizeof(buffer), "%a %b %d %Y at %H:%M:%S GMT", timeStruct);
	return std::string(buffer);
}

std::vector<std::string>	vsplit(const std::string & str, char delimiter)
{
	std::vector<std::string>	result;
	std::string					token;

	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] == delimiter && !token.empty())
		{
			result.push_back(token);
			token.clear();
		}
		else if (str[i] != delimiter)
			token += str[i];  
	}
	if (!token.empty())
		result.push_back(token);
	return result;
}

std::string itoa(int nb)
{
	std::ostringstream oss;
	oss << nb;
	return (std::string) oss.str();
}

bool charIsNot(char c, const std::string &plage)
{
	for (std::string::const_iterator iter = plage.begin(); iter != plage.end(); iter++)
		if (c == *iter)
			return true;
	return false;
}

bool strCompareNoCase(const std::string &string1, const std::string &string2)
{
	if (string1.size() != string2.size())
		return false;
	for (size_t i = 0; i < string1.size(); i++)
		if (tolower(string1[i]) != tolower(string2[i]))
			return false;
	return true;
}
