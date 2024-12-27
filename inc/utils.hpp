/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 17:44:33 by aduvilla          #+#    #+#             */
/*   Updated: 2024/12/27 14:33:07 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP
# include <vector>
# include "Channel.hpp"

#include <vector>
#include <string>
class Server;
class Client;

//int	sendMessage(const int fd, const std::string & source, const std::string & command, const std::string msg);
void	sendMessage(const int fd, const std::string & source, const std::string & command, const std::string msg);
std::vector<std::string>	vsplit(const std::string & str, char delimiter);
const std::string	getTime();
std::vector<std::string>	vsplit(const std::string & str, char delimiter);
std::string itoa(int nb);
bool charIsNot(char c, const std::string &plage);
bool strCompareNoCase(const std::string &string1, const std::string &string2);
//int sendf(Server *server, Client *client, const std::string str, ...);
void sendf(Server *server, Client *client, const std::string str, ...);
std::string getMode(Mode mode);
#endif
