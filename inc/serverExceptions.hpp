/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverExceptions.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 17:28:06 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/21 08:22:24 by noda             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXCEPTIONS_HPP
# define EXCEPTIONS_HPP
# include <exception>
# include <string>

class Server;
class Client;
class serverExceptions : public std::exception
{
	private : 
		const int	m_errorCode;
	public :
		int getErrorCode() const;
		serverExceptions(const int errorCode); 
		void sendError(Server &server, Client *dest, ...) const ;
}	;

#endif