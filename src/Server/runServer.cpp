/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 16:09:09 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/14 15:39:27 by aduvilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parsing.hpp"
#include "Server.hpp"
#include <fcntl.h>
#include <arpa/inet.h>
#include <string>
#include <unistd.h>
#include <string.h>
#include "UserHost.hpp"
#include "Ping.hpp"
#include "Topic.hpp"
#include "PrivMsg.hpp"
#include "Notice.hpp"
#include "Join.hpp"
#include "Nick.hpp"
#include "Part.hpp"
#include "Kick.hpp"
#include "Mode.hpp"
#include "Invite.hpp"
#include "Quit.hpp"
#include "serverExceptions.hpp"
#include "utils.hpp"
#include "Numerics.hpp"
#include <map>

void Server::AcceptNewClient()
{
	Client *client = new Client();
	struct sockaddr_in clientAdd;
	struct pollfd newPoll;
	socklen_t len = sizeof(clientAdd);

	int incoFd = accept(m_serverSocketFd, reinterpret_cast<struct sockaddr*>(&clientAdd), &len);
	if (incoFd == -1)
	{
		sendLog("Error: accept failed");
		return;
	}
	if (fcntl(incoFd, F_SETFL, O_NONBLOCK) == -1)
	{
		sendLog("Error: fcntl failed");
		return;
	}
	newPoll.fd = incoFd;
	newPoll.events = POLLIN;
	newPoll.revents = 0;
	
	client->setFD(incoFd);
	client->setIPadd(inet_ntoa((clientAdd.sin_addr)));
	client->setHostname(reinterpret_cast<struct sockaddr*>(&clientAdd), *this);
	// m_mClients.insert({incoFd, client});
	m_vClients.push_back(client);
	m_vFds.push_back(newPoll);
	sendLog("Client <" + itoa(incoFd) + "> Connected");
}

void Server::ReceiveNewData(Client &client)
{
	char buff[513]; //-> buffer for the received data
	memset(buff, 0, sizeof(buff)); //-> clear the buffer
	ssize_t bytes = recv(client.getFD(), buff, sizeof(buff) - 1, 0); //-> receive the data

	if (bytes <= 0){ //-> check if the client disconnected
		sendLog(static_cast<std::string>("Client <" + itoa(client.getFD()) + "> Disconnected"));
		ClearClient(client); //-> clear the client
	}
	else //-> print the received data
	{
		buff[bytes] = '\0';
		std::string line = parseBuffer(client, buff);
		if (checkAuth(client, line))
		{
			size_t j = 0;
			for (size_t k = 0; k < line.size(); k = j + 1)
			{
				j = line.find("\n", k + 1);
				parseCommand(line.substr(k, j - k - 1), client);
			}
		}
	}
}

void Server::parseCommand(const std::string line, Client &client)
{
	typedef void (*CommandFunction)(Server &, const Parsing &, Client &);
	std::map<std::string, CommandFunction> commandMap;

	commandMap["JOIN"] = &Join::execute;
	commandMap["NICK"] = &Nick::execute;
	commandMap["userhost"] = &UserHost::execute;
	commandMap["PING"] = &Ping::execute;
	commandMap["PRIVMSG"] = &PrivMsg::execute;
	commandMap["NOTICE"] = &Notice::execute;
	commandMap["PART"] = &Part::execute;
	commandMap["TOPIC"] = &Topic::execute;
	commandMap["KICK"] = &Kick::execute;
	commandMap["MODE"] = &Mode::execute;
	commandMap["INVITE"] = &Invite::execute;
	commandMap["QUIT"] = &Quit::execute;
//	commandMap["WHO"] = &Who::execute;
	sendLog(itoa(client.getFD()) + " >> " + line);
	Parsing parse(line);
	std::map<std::string, CommandFunction>::iterator it = commandMap.find(parse.getCommand());
	if (it != commandMap.end())
		it->second(*this, parse, client);
	else
		this->sendf(&client, NULL, NULL, ERR_UNKNOWNCOMMAND, parse.getCommand().c_str());
}

std::string Server::parseBuffer(Client &client, std::string buffer)
{
	if (buffer.find("\n") == buffer.npos)
	{
		client.addPacket(buffer);
		return "";
	}
	size_t	pos = 0;
	while ((pos = buffer.find('\n', pos)) != std::string::npos)
	{
		if (pos == 0 || buffer[pos - 1] != '\r')
		{
			buffer.insert(pos, 1, '\r');
			pos++;
		}
		pos++;
	}
	std::string result = client.getPacket() + buffer; //careful this one might bring bugs if /r/n is not the end of the string
	size_t	lastN = result.rfind('\n');
	client.addPacket(result.substr(lastN + 1));
	return result.substr(0, lastN + 1);
}
/*
void Server::parseCommand(const std::string line, Client &client)
{
	bool	executed = false;
	std::string		Commands[] = {"JOIN", 
	"NICK", 
	"userhost", 
	"PING", 
	"PRIVMSG", 
	"NOTICE", 
	"PART", 
	"TOPIC", 
	"KICK", 
	"MODE",
	"INVITE",
	"QUIT"};
	void (*fCommands[])(Server &, const Parsing &, Client &) = { &Join::execute,
		&Nick::execute,
		&UserHost::execute,
		&Ping::execute,
		&PrivMsg::execute,
		&Notice::execute,
		&Part::execute,
		&Topic::execute,
		&Kick::execute,
		&Mode::execute,
		&Invite::execute,
		&Quit::execute};
	size_t size = sizeof(Commands) / sizeof(Commands[0]);
	sendLog(itoa(client.getFD()) + " >> " + line);
	Parsing parse(line);
	for (size_t i = 0; i < size; i++)
	{
		if (parse.getCommand() == Commands[i])
		{
			fCommands[i](*this, parse, client);
			executed = true;
		}
	}
	if (!executed)
		this->sendf(&client, NULL, NULL, ERR_UNKNOWNCOMMAND, parse.getCommand().c_str());
}
*/
