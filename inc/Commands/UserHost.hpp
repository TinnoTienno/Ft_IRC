/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserHost.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 15:34:29 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/09 15:34:38 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"

class UserHost
{
	public :
		static void execute(Server *server, const std::string buffer, Client &client);
}	;