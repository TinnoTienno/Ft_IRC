/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Notice.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduvilla <aduvilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 23:57:08 by aduvilla          #+#    #+#             */
/*   Updated: 2024/12/16 23:57:53 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NOTICE_HPP
# define NOTICE_HPP
# include "Parsing.hpp"

class Server;
class Client;

class	Notice
{
	public :
		static void execute(Server *server, const Parsing &parse, Client &client);
};

#endif
