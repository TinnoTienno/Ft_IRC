/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 15:38:29 by eschussl          #+#    #+#             */
/*   Updated: 2024/11/22 17:14:10 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP
# include <string>

class Client
{
	private :
		int 		m_fd;
		std::string m_ipAdd;
	public : 
		Client();
		const int& getFD() const;
		void setFD(const int&);
		void setIPadd(const std::string&);
		~Client();
}	;

#endif