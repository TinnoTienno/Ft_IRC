 #ifndef BOT_HPP
#define BOT_HPP

#include <string>
#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <stdexcept>
#include <vector>
#include <sstream>
#include <fstream>

class Bot {
public:
    Bot(const std::string& serAdd, const std::string& botname, const std::string& password, int port);
    ~Bot();

    std::string getNick() const;
    std::string getUsername() const;
    std::string getRealname() const;
    int init();
    int quit();
    void sendCommand(const std::string& msg) const;

private:
    void connectToServer();
    void authenticate();
    int m_run();
    int handleDccSend(const std::string& user, const std::string& filename) const;
    std::string getLocalIp() const;
    void trimNewlines(std::string& str) const;
    uint32_t convertIpToInt(const std::string& ip_str) const;
    void handlePrivMsg(const std::string& message);

    std::string m_serAddress;
    std::string m_name;
    std::string m_password;
    int m_port;
    int m_serSocket;
    bool m_signal;
};

#endif

