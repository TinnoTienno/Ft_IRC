/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 17:10:50 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/09 18:34:26 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"

class Nick
{
	public :
		static void execute(Server *server, const std::string buffer, Client &client);
		static int checkformat(Server *server, const std::string &buffer);
}	;