/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduvilla <aduvilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 15:23:07 by aduvilla          #+#    #+#             */
/*   Updated: 2024/12/05 15:24:56 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <string>

class	Channel
{
	public:
		Channel(void);
		Channel(Channel const & src);
		Channel &operator=(Channel const & rhs);
		~Channel(void);

	private:
		std::string	m_name;
		std::string	m_password;
};

#endif
