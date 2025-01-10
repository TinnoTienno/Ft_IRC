/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 23:58:31 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/09 12:15:15 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Notice.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "Parsing.hpp"
#include "utils.hpp"
#include "serverExceptions.hpp"

void	Notice::execute(Server &server, const Parsing &parse, Client &client)
{
	if (parse.getArguments().size() < 3)
		return ;
	std::vector<std::string>	targets = vsplit(parse.getArguments()[1], ',');
	for (size_t i = 0; i < targets.size(); i++)
	{
		if (targets[i].find('#') == 0)
		{
			Channel *chan = server.getChannel(targets[i]);
			if (chan)
				chan->sendAllMsg(&server, &client, parse.getArguments()[2], eNotice); // !!!! il faut aussi envoyer le client actuel pour la source du msg
		}
		else
	  	{
			Client *user = server.getClient( targets[i]);
			if (user)
				server.sendf(user, &client, NULL, NOTICE, parse.getArguments()[2].c_str());
		}
	}
}
