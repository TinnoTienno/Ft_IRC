/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Who.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduvilla <aduvilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 15:52:02 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/14 16:12:45 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WHO_HPP
# define WHO_HPP

class Server;
class Client;
class Parsing;

class Who
{
	public :
		static void execute(Server &server, const Parsing &parse, Client &client);
};

#endif
