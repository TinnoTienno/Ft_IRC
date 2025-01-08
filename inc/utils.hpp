/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 17:44:33 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/08 16:34:06 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include <vector>
# include "Channel.hpp"
# include <vector>
# include <string>
# define NOTICE ":%s NOTICE %n :%m" // source, nickname, message
# define QUIT ":%s QUIT :Quit: %m" // source, message

class Server;
class Client;

//int	sendMessage(const int fd, const std::string & source, const std::string & command, const std::string msg);
void	sendMessage(const int fd, Server &server, const std::string & command, const std::string msg);
const std::string			getTime();
std::vector<std::string>	vsplit(const std::string & str, char delimiter);
std::string					itoa(int nb);
bool 						charIsNot(char c, const std::string &plage);
bool 						strCompareNoCase(const std::string &string1, const std::string &string2);
void 						sendf(Server *server, Client *client, const std::string str, ...);
#endif
