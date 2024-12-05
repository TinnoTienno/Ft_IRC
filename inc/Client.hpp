/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 15:38:29 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/05 18:18:53 by eschussl         ###   ########.fr       */
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
		bool		m_authentified;
	public : 
		Client();
		const int& getFD() const;
		void setFD(const int&);
		void setIPadd(const std::string&);
		void setAuth(const bool &);
		const bool& getAuth() const;
		~Client();
}	;

#endif