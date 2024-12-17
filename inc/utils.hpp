/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 17:44:33 by aduvilla          #+#    #+#             */
/*   Updated: 2024/12/17 17:34:31 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP
# include <vector>

#include <string>

void	sendMessage(const int fd, const std::string & source, const std::string & command, const std::string msg);
const std::string	getTime();
std::vector<std::string>	vsplit(const std::string & str, char delimiter);
std::string itoa(int nb);
bool charIsNot(char c, const std::string &plage);
bool strCompareNoCase(const std::string &string1, const std::string &string2);
#endif
