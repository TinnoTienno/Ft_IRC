#include "Bot.hpp"
#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <stdexcept>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>

Bot::Bot(std::string serAdd, std::string botname, std::string password, int port) : m_serAddress(serAdd), m_name(botname), m_password(password), m_port(port), m_signal(false) {}

Bot::~Bot() {}

std::string	Bot::getNick() const { return this->m_name; }

std::string	Bot::getUsername() const { return "U-" + this->m_name; }

std::string	Bot::getRealname() const { return "R-" + this->m_name; }

int	Bot::init()
{
	struct sockaddr_in	socketAdd;

	socketAdd.sin_family = AF_INET;
	socketAdd.sin_port = htons(this->m_port);
	socketAdd.sin_addr.s_addr = inet_addr(this->m_serAddress.c_str());

	this->m_serSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(this->m_serSocket == -1)
		throw std::runtime_error("Error: Socket failed");
	if(connect(this->m_serSocket, (struct sockaddr*)&socketAdd, (socklen_t)(sizeof(socketAdd))) != 0)
		throw std::runtime_error("Error: Cannot connect to server");
	if(speak("PASS " + this->m_password + "\r\n"))
		return this->quit();
	if(speak("NICK " + this->m_name + "\r\n"))
		return this->quit();
	if(speak("USER U-" + this->m_name + " 0 * :R-" + this->m_name + "\r\n"))
		return this->quit();
	if(this->m_run())
		return this->quit();
	return 0;
}

int	Bot::m_run()
{
	char	buffer[513];

	while(this->m_signal == false)
	{
		memset(buffer, 0, sizeof(buffer));
		int	bytes = recv(this->m_serSocket, buffer, sizeof(buffer), 0);
		if (bytes <= 0)
			return 1;
		buffer[bytes] = '\0';
		std::cout << buffer << std::endl;
         std::string message(buffer);

        if (message.find("PRIVMSG") != std::string::npos)
//      if (message.find("PRIVMSG") == 0)
		{
			std::stringstream ss(message);
			std::string token;
			std::vector<std::string> tokens;

			while (std::getline(ss, token, ' '))
				tokens.push_back(token);
			if (tokens.size() > 3 && tokens[3] == ":!send")
			{
				std::string user = tokens[0].substr(1, tokens[0].find("!")-1);
				if (tokens.size() > 4)
				{
					std::string filename = tokens[4];
					while (!filename.empty() && (filename[filename.size() - 1] == '\r' || filename[filename.size() - 1] == '\n'))
						filename.erase(filename.size() - 1, 1);
					handle_dcc_send(user, filename);

				}
				else
					speak("PRIVMSG " + user + " :Usage: !send filename\r\n");
			}
		}
	}
	return 0;
}


uint32_t Bot::ip_to_int(const std::string& ip_str) {
    struct in_addr addr;
    if (inet_pton(AF_INET, ip_str.c_str(), &addr) == 1) {
        return ntohl(addr.s_addr);
    }
    return 0; // Error
}


std::string Bot::get_local_ip() {
  int sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock == -1) {
    return "0.0.0.0";
  }
  sockaddr_in loopback;
    memset(&loopback, 0, sizeof(loopback));
    loopback.sin_family = AF_INET;
    loopback.sin_addr.s_addr = inet_addr("127.0.0.1");
    loopback.sin_port = htons(9);
    if (connect(sock, (sockaddr*)&loopback, sizeof(loopback)) == -1) {
        close(sock);
        return "0.0.0.0";
    }
    sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    if (getsockname(sock, (sockaddr*)&addr, &addr_len) == -1) {
        close(sock);
        return "0.0.0.0";
    }
    close(sock);

    char ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(addr.sin_addr), ip_str, INET_ADDRSTRLEN);
    return ip_str;

}

int Bot::handle_dcc_send(const std::string& user, const std::string& filename) {
      std::string local_ip = get_local_ip();
    if (local_ip == "0.0.0.0") {
        speak("PRIVMSG " + user + " : Error getting local IP.");
        return 1;
    }
    // 1. Ouvrir le fichier
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        speak("PRIVMSG " + user + " :Error opening file.\r\n");
        return 1;
    }

    size_t filesize = file.tellg();
    file.seekg(0, std::ios::beg);

    // 2. Créer un socket serveur TCP
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        speak("PRIVMSG " + user + " :Error creating server socket.");
        return 1;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = 0; // Port 0 pour que le systeme choisisse un port disponible

    if (bind(server_sock, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
         speak("PRIVMSG " + user + " :Error binding socket.");
         close(server_sock);
        return 1;
    }
   socklen_t addrlen = sizeof(server_addr);
   if (getsockname(server_sock, (sockaddr *)&server_addr, &addrlen) == -1) {
        speak("PRIVMSG " + user + " :Error getting port");
        close(server_sock);
        return 1;
    }

    int port = ntohs(server_addr.sin_port);

     if (listen(server_sock, 1) == -1) {
        speak("PRIVMSG " + user + " :Error listening on socket.");
        close(server_sock);
        return 1;
     }

    // 3. Envoyer la commande DCC au client
    std::string dcc_message = "PRIVMSG " + user + " :DCC SEND \"" + filename + "\" " +
                              std::to_string(ip_to_int(local_ip)) + " " +
                              std::to_string(port) + " " +
                              std::to_string(filesize) + "\r\n";

     speak(dcc_message);

    // 4. Accepter la connexion
    sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int client_sock = accept(server_sock, (sockaddr*)&client_addr, &client_addr_len);

    if (client_sock == -1) {
         speak("PRIVMSG " + user + " :Error accepting connection.");
        close(server_sock);
        return 1;
    }

    // 5. Envoyer le fichier
    char buffer[4096];
    size_t total_sent = 0;
    while (total_sent < filesize) {
        file.read(buffer, sizeof(buffer));
        size_t bytes_read = file.gcount();
        if(send(client_sock, buffer, bytes_read, 0) == -1){
           speak("PRIVMSG " + user + " :Error sending file");
          close(server_sock);
          close(client_sock);
          return 1;
        }
        total_sent += bytes_read;
    }


    // 6. Fermer les sockets
    close(client_sock);
    close(server_sock);
     speak("PRIVMSG " + user + " :File transfer complete.");
     return 0;
}

int	Bot::speak(const std::string & msg)
{
	std::cout << msg << std::endl;
	if (send(this->m_serSocket, msg.c_str(), msg.size(), 0) != (ssize_t)msg.length())
	{
		std::cerr << "Error: Message not sent" << std::endl;
		return 1;
	}
	return 0;
}

int	Bot::quit()
{
	close(this->m_serSocket);
	std::cout << "ircbot Disconnected" << std::endl;
	return 1;
}
