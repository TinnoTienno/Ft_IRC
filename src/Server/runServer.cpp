/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 16:09:09 by aduvilla          #+#    #+#             */
/*   Updated: 2025/01/17 14:51:37 by noda             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parsing.hpp"
#include "Server.hpp"
#include <fcntl.h>
#include <arpa/inet.h>
#include <string>
#include <unistd.h>
#include <string.h>
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
#include "Who.hpp"
#include "serverExceptions.hpp"
#include "utils.hpp"
#include "Numerics.hpp"
#include <map>

/**
 * @brief  Accepts a new client connection and sets up the necessary structures.
 *
 * This function creates a new client, accepts the incoming connection, and
 * configures the necessary socket and poll structures to handle the client.
 */
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


/**
 * @brief  Receives and processes new data from a connected client.
 *
 * This function reads data sent by the client, checks for disconnection,
 * parses the buffer for commands, and processes those commands if the client
 * is authenticated.
 *
 * @param  client : Reference to the client sending data.
 */
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
				handleCommand(line.substr(k, j - k - 1), client);
			}
		}
	}
}

/**
 * @brief  Handles a command sent by a client.
 *
 * This function maps incoming command strings to their corresponding
 * command-handling functions and executes the appropriate action.
 *
 * @param  line   : The command string received from the client.
 * @param  client : Reference to the client sending the command.
 */
void Server::handleCommand(const std::string line, Client &client)
{
	typedef void (*CommandFunction)(Server &, const Parsing &, Client &);
	std::map<std::string, CommandFunction> commandMap;

	commandMap["JOIN"] = &Join::execute;
	commandMap["NICK"] = &Nick::execute;
	commandMap["PING"] = &Ping::execute;
	commandMap["PRIVMSG"] = &PrivMsg::execute;
	commandMap["NOTICE"] = &Notice::execute;
	commandMap["PART"] = &Part::execute;
	commandMap["TOPIC"] = &Topic::execute;
	commandMap["KICK"] = &Kick::execute;
	commandMap["MODE"] = &Mode::execute;
	commandMap["INVITE"] = &Invite::execute;
	commandMap["QUIT"] = &Quit::execute;
	commandMap["WHO"] = &Who::execute;
	sendLog(itoa(client.getFD()) + " >> " + line);
	Parsing parse(line);
	std::map<std::string, CommandFunction>::iterator it = commandMap.find(parse.getCommand());
	if (it != commandMap.end())
		it->second(*this, parse, client);
	else
		this->sendf(&client, NULL, NULL, ERR_UNKNOWNCOMMAND, parse.getCommand().c_str());
}

/**
 * @brief  Parses the incoming message buffer and handles newline characters.
 *
 * This function processes the message buffer by adding necessary carriage
 * returns before newline characters and stores the complete message in
 * the client's packet for further processing.
 *
 * @param  client  : Reference to the client whose data is being parsed.
 * @param  message : The raw message string received from the client.
 *
 * @return A parsed string containing the message data without the newline.
 */
std::string Server::parseBuffer(Client &client, std::string message)
{
	if (message.find("\n") == message.npos)
	{
		client.addPacket(message);
		return "";
	}
	size_t	pos = 0;
	while ((pos = message.find('\n', pos)) != std::string::npos)
	{
		if (pos == 0 || message[pos - 1] != '\r')
		{
			client.setNetCat(true);
			message.insert(pos, 1, '\r');
			pos++;
		}
		pos++;
	}
	std::string result = client.getPacket() + message; //careful this one might bring bugs if /r/n is not the end of the string
	size_t	lastN = result.rfind('\n');
	client.addPacket(result.substr(lastN + 1));
	return result.substr(0, lastN + 1);
}
