/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduvilla <aduvilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 08:27:45 by aduvilla          #+#    #+#             */
/*   Updated: 2024/12/27 10:47:57 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __BOT_HPP__
# define __BOT_HPP__
# include <string>
# define USAGE "Usage: ./bot [server adress] [name] [password] [port]"

class	Bot
{
	public:
		Bot	(std::string serAdd, std::string botname, std::string password, int port);
		int	init();
		int	speak(const std::string & msg);
		int	quit();
		std::string	getNick() const;
		std::string getUsername() const;
		std::string getRealname() const;
		~Bot(void);

	private:
		std::string	m_serAddress;
		std::string	m_name;
		std::string	m_password;
		int			m_serSocket;
		int			m_port;
		int			m_run();
		bool		m_signal;
};

#endif /* __BOT_HPP__ */
