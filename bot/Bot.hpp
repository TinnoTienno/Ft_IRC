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
    Bot(std::string serAdd, std::string botname, std::string password, int port);
    ~Bot();

    std::string getNick() const;
    std::string getUsername() const;
    std::string getRealname() const;
    int init();
    int quit();
    int speak(const std::string & msg);

private:
    int m_run();
    int handle_dcc_send(const std::string& user, const std::string& filename);
     uint32_t ip_to_int(const std::string& ip_str);
    std::string get_local_ip();
    std::string m_serAddress;
    std::string m_name;
    std::string m_password;
    int m_port;
    int m_serSocket;
    bool m_signal;
};

#endif
