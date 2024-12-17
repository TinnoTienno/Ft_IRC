/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 17:28:06 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/17 16:34:07 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXCEPTIONS_HPP
# define EXCEPTIONS_HPP
# include <exception>
# include <string>

class serverExceptions : public std::exception
{
	private : 
		const int	m_errorCode;
	public :
		int getErrorCode() const;
		serverExceptions(const int errorCode);
}	;

#endif