/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduvilla <aduvilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 23:58:31 by aduvilla          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/12/17 17:38:08 by eschussl         ###   ########.fr       */
=======
/*   Updated: 2024/12/17 15:40:52 by aduvilla         ###   ########.fr       */
>>>>>>> 2aef5294e1965bb4b2a8ba74b2a0886033821d3f
/*                                                                            */
/* ************************************************************************** */

#include "Notice.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "Parsing.hpp"
#include "utils.hpp"

void	Notice::execute(Server *server, const Parsing &parse, Client &client)
{
	if (parse.getArguments().size() < 3)
		return ;
	std::vector<std::string>	targets = vsplit(parse.getArguments()[1], ',');
	for (size_t i = 0; i < targets.size(); i++)
	{
		if (targets[i].find('#') == 0)
		{
			Channel *chan = server->getChannel(targets[i].substr(1));
			if (chan)
				chan->sendAllMsg(parse.getArguments()[2]); // !!!! il faut aussi envoyer le client actuel pour la source du msg
		}
		else
	  	{
			Client *user = server->getClient( targets[i]);
			if (user)
				sendMessage(user->getFD(), client.getPrefix(), "NOTICE " + targets[i], parse.getArguments()[2]);
		}
	}
}
