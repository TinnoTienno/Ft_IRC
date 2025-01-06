/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 13:44:48 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/21 13:55:15 by noda             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parsing.hpp"
#include <iostream>
#include "serverExceptions.hpp"

Parsing::Parsing(std::string line)
{
	size_t wordIndex = 0;
	size_t i = 0;
	while (i != line.npos)
	{
		std::string word = line.substr(i, line.find(' ', i) - i);
		if (word[0] == ':')
		{
			word = line.substr(i + 1, line.npos - i);
			i = line.npos;
		}
		else if (line.find(' ', i) != line.npos)
			i = line.find(' ', i) + 1;
		else
			i = line.npos;
		m_arguments.push_back(word);
		wordIndex++;
	}
}

const std::vector<std::string> &Parsing::getArguments() const {	return m_arguments; }

const std::string &Parsing::getCommand() const { return m_arguments[0]; }
