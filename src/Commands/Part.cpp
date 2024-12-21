/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 14:19:25 by noda              #+#    #+#             */
/*   Updated: 2024/12/21 15:17:10 by noda             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Part.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "Parsing.hpp"
#include "utils.hpp"
#include <iostream>
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
				e.sendError(server, &client, parse.getCommand().c_str());
				break;
        }
    }
    std::cout << "yo " << parse.getArguments()[1] << std::endl;
	std::vector<std::string> channelnames = vsplit(parse.getArguments()[1], ',');
    Channel *chan;
    for (std::vector<std::string>::iterator iter = channelnames.begin(); iter != channelnames.end(); iter++)
    {
        try
        {
            chan = server.getChannel(*iter);
            if (!chan)
                throw serverExceptions(403);
            Client *cli = chan->getClient(&client);
            if (!cli)
                throw serverExceptions(442);
            if (parse.getArguments().size() == 30)
                chan->sendPart(client, parse.getArguments()[2]);
            else
                chan->sendPart(client, "");
            chan->removeClient(server, *cli);
            std::cout << "coucou" << std::endl;
        }
        catch(const serverExceptions& e)
        {
            switch (e.getErrorCode())
            {
                case 403 :
			    	e.sendError(server, &client, *iter);
			    	break;
                case 442 :
                    e.sendError(server, &client, chan->getName());
                    break;
            }
			
        }
        
    }
}