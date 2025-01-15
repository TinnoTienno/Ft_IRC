/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 14:19:25 by noda              #+#    #+#             */
/*   Updated: 2025/01/14 18:08:30 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Part.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "Parsing.hpp"
#include "utils.hpp"
#include "serverExceptions.hpp"

void Part::execute(Server &server, const Parsing &parse, Client &client)
{
    try 
    {
        if (parse.getArguments().size() < 2)
            throw serverExceptions(461);
    }
    catch (const serverExceptions&e)
    {
        switch (e.getErrorCode())
        {
            case 461 :
				e.sendError(server, &client, NULL, parse.getCommand().c_str());
				break;
        }
    }
	std::vector<std::string> channelnames = vsplit(parse.getArguments()[1], ',');
    Channel *chan;
    for (std::vector<std::string>::iterator iter = channelnames.begin(); iter != channelnames.end(); iter++)
    {
        try
        {
            chan = server.getChannel(*iter);
            if (!chan)
                throw serverExceptions(403);
            Client *target = chan->getClient(&client);
            if (!target)
                throw serverExceptions(442);
            if (parse.getArguments().size() == 30)
//                chan->sendPart(client, parse.getArguments()[2]);
                chan->sendAllMsg(&server, &client, parse.getArguments()[2], ePart);
            else
                chan->sendAllMsg(&server, &client, "", ePart);
            chan->removeClient(*target);
        }
        catch(const serverExceptions& e)
        {
            switch (e.getErrorCode())
            {
                case 403 :
			    	e.sendError(server, &client, NULL, (*iter).c_str());
			    	break;
                case 442 :
                    e.sendError(server, &client, chan);
                    break;
            }
			
        }
        
    }
}
