#ifndef BOT_HPP
#define BOT_HPP

#include <string>
#include <arpa/inet.h>
#include <netinet/in.h>

class Bot {
public:
    Bot(std::string serAdd, std::string botname, std::string password, int port);
    ~Bot();

    std::string getNick() const;
    std::string getUsername() const;
    std::string getRealname() const;
    int init();
    int quit();
    void speak(const std::string & msg);

private:
	void	m_authenticate();
	void	m_connectToServer();
    int		m_run();
	void	handlePrivMsg(const std::string & message);
	uint32_t	m_getLocalIpInt() const;
    int		handle_dcc_send(const std::string& user, const std::string& filename);
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
