/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 13:40:11 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/13 14:22:34 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_HPP
#define PARSING_HPP
#include <string>
#include <vector>
class Parsing
{
	private :
		std::vector<std::string> m_arguments;
	public :
		Parsing(std::string line);
		const std::string &getCommand() const;
		const std::vector<std::string> &getArguments() const;
}	;

#endif