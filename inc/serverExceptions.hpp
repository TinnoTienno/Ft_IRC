/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverExceptions.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 17:28:06 by eschussl          #+#    #+#             */
/*   Updated: 2025/01/13 17:05:55 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXCEPTIONS_HPP
# define EXCEPTIONS_HPP
# include <exception>
# include <string>

class Channel;
class Server;
class Client;
class serverExceptions : public std::exception
{
	private : 
		const int	m_errorCode;
	public :
		int getErrorCode() const;
		serverExceptions(const int errorCode); 
		void sendError(Server &server, Client *dest, Channel *channel, ...) const;
}	;

#endif