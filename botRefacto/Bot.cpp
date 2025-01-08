 
#include "Bot.hpp"
#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <stdexcept>
#include <string>
#include <cstring>
#include <sstream>
#include <fstream>
#include <vector>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>

Bot::Bot(const std::string& serAdd, const std::string& botname, const std::string& password, int port)
    : m_serAddress(serAdd), m_name(botname), m_password(password), m_port(port), m_signal(false), m_serSocket(-1) {}

Bot::~Bot() {
    if (m_serSocket != -1) {
        close(m_serSocket);
    }
}

std::string Bot::getNick() const { return m_name; }

std::string Bot::getUsername() const { return "U-" + m_name; }

std::string Bot::getRealname() const { return "R-" + m_name; }

void Bot::connectToServer() {
    struct sockaddr_in socketAdd;

    socketAdd.sin_family = AF_INET;
    socketAdd.sin_port = htons(m_port);
    socketAdd.sin_addr.s_addr = inet_addr(m_serAddress.c_str());

    m_serSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_serSocket == -1) {
        throw std::runtime_error("Error: Socket creation failed");
    }

    if (connect(m_serSocket, (struct sockaddr*)&socketAdd, sizeof(socketAdd)) != 0) {
        throw std::runtime_error("Error: Cannot connect to server");
    }
}

void Bot::authenticate() {
    sendCommand("PASS " + m_password);
    sendCommand("NICK " + m_name);
    sendCommand("USER " + getUsername() + " 0 * :" + getRealname());
}

int Bot::init() {
    try {
        connectToServer();
        authenticate();
        return m_run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return quit();
    }
}

int Bot::m_run() {
    char buffer[513];

    while (!m_signal) {
        memset(buffer, 0, sizeof(buffer));
        int bytes = recv(m_serSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytes <= 0) {
            return 1;
        }
        buffer[bytes] = '\0';
        std::string message(buffer);
        std::cout << message << std::endl;

        if (message.find("PRIVMSG") != std::string::npos) {
            handlePrivMsg(message);
        }
    }
    return 0;
}

void Bot::handlePrivMsg(const std::string& message) {
    std::istringstream ss(message);
    std::string token;
    std::vector<std::string> tokens;

    while (std::getline(ss, token, ' ')) {
        tokens.push_back(token);
    }

    if (tokens.size() > 3 && tokens[3] == ":!send") {
        std::string user = tokens[0].substr(1, tokens[0].find("!") - 1);
        if (tokens.size() > 4) {
            std::string filename = tokens[4];
            trimNewlines(filename);
            handleDccSend(user, filename);
        } else {
            sendCommand("PRIVMSG " + user + " :Usage: !send filename");
        }
    }
}

void Bot::trimNewlines(std::string& str) const {
    while (!str.empty() && (str.back() == '\r' || str.back() == '\n')) {
        str.pop_back();
    }
}

uint32_t Bot::convertIpToInt(const std::string& ip_str) const {
    struct in_addr addr;
    if (inet_pton(AF_INET, ip_str.c_str(), &addr) == 1) {
        return ntohl(addr.s_addr);
    }
    throw std::runtime_error("Error: Invalid IP address");
}

std::string Bot::getLocalIp() const {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        return "0.0.0.0";
    }

    struct sockaddr_in loopback = {};
    loopback.sin_family = AF_INET;
    loopback.sin_addr.s_addr = inet_addr("127.0.0.1");
    loopback.sin_port = htons(9);

    if (connect(sock, (struct sockaddr*)&loopback, sizeof(loopback)) == -1) {
        close(sock);
        return "0.0.0.0";
    }

    struct sockaddr_in addr = {};
    socklen_t addr_len = sizeof(addr);
    if (getsockname(sock, (struct sockaddr*)&addr, &addr_len) == -1) {
        close(sock);
        return "0.0.0.0";
    }

    close(sock);

    char ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &addr.sin_addr, ip_str, sizeof(ip_str));
    return ip_str;
}

int Bot::handleDccSend(const std::string& user, const std::string& filename) const {
    try {
        std::string local_ip = getLocalIp();
        uint32_t ip_int = convertIpToInt(local_ip);

        std::ifstream file(filename.c_str(), std::ios::binary | std::ios::ate);
        if (!file.is_open()) {
            sendCommand("PRIVMSG " + user + " :Error opening file.");
            return 1;
        }

        size_t filesize = file.tellg();
        file.seekg(0, std::ios::beg);

        int server_sock = socket(AF_INET, SOCK_STREAM, 0);
        if (server_sock == -1) {
            throw std::runtime_error("Error: Socket creation failed");
        }

        struct sockaddr_in server_addr = {};
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = 0;

        if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
            close(server_sock);
            throw std::runtime_error("Error: Socket bind failed");
        }

        socklen_t addrlen = sizeof(server_addr);
        if (getsockname(server_sock, (struct sockaddr*)&server_addr, &addrlen) == -1) {
            close(server_sock);
            throw std::runtime_error("Error: Getting socket name failed");
        }

        int port = ntohs(server_addr.sin_port);
        if (listen(server_sock, 1) == -1) {
            close(server_sock);
            throw std::runtime_error("Error: Listening on socket failed");
        }

        std::ostringstream dcc_message;
        dcc_message << "PRIVMSG " << user << " :\001DCC SEND " << filename << " " << ip_int << " " << port << " " << filesize << "\001";
        sendCommand(dcc_message.str());

        struct sockaddr_in client_addr = {};
        socklen_t client_addr_len = sizeof(client_addr);
        int client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_addr_len);
        if (client_sock == -1) {
            close(server_sock);
            throw std::runtime_error("Error: Accepting connection failed");
        }

        char buffer[4096];
        size_t total_sent = 0;
        while (total_sent < filesize) {
            file.read(buffer, sizeof(buffer));
            size_t bytes_read = file.gcount();
            if (send(client_sock, buffer, bytes_read, 0) == -1) {
                close(client_sock);
                close(server_sock);
                throw std::runtime_error("Error: Sending file failed");
            }
            total_sent += bytes_read;
        }

        close(client_sock);
        close(server_sock);
        sendCommand("PRIVMSG " + user + " :File transfer complete.");
        return 0;
    } catch (const std::exception& e) {
        sendCommand("PRIVMSG " + user + " :" + std::string(e.what()));
        return 1;
    }
}

void Bot::sendCommand(const std::string& msg) const{
    std::cout << msg << std::endl;
    if (send(m_serSocket, msg.c_str(), msg.size(), 0) != static_cast<ssize_t>(msg.length())) {
        throw std::runtime_error("Error: Message not sent");
    }
}

int Bot::quit() {
    if (m_serSocket != -1) {
        close(m_serSocket);
        m_serSocket = -1;
    }
    std::cout << "ircbot Disconnected" << std::endl;
    return 1;
}
