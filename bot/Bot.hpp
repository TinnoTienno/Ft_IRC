#ifndef BOT_HPP
#define BOT_HPP

#include <string>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <vector>

class Bot {
public:
    Bot(std::string serAdd, std::string channel, std::string password, int port);
    ~Bot();

    std::string getNick() const;
    std::string getUsername() const;
    std::string getRealname() const;
    int init();
    int quit();
	static void	signalHandler(int signum);
    void speak(const std::string & msg);

private:
    static bool 				m_signal;
    std::string 				m_serAddress;
    std::string 				m_name;
	std::string					m_channel;
    std::string 				m_password;
	std::string					m_fileDir;
	std::vector<std::string>	m_vlist;
    int 						m_port;
    int 						m_serSocket;
	void		m_authenticate();
	void		m_connectToServer();
	void		m_createList();
    int			m_run();
	void		m_handlePrivMsg(const std::string & message);
	uint32_t	m_getLocalIpInt() const;
	void		m_createSocket(struct sockaddr_in & serverAddr, int & serverSock);
	void		m_acceptAndSend(int serverSock, const std::string & user, std::ifstream & file);
	void		m_handleList(const std::string & user);
	bool		isInList(const std::string & user, const std::string & filename);
    int			m_handleSendFile(const std::string& user, const std::string& filename);
	std::string&	m_trimNewLines(std::string & str);
};

#endif
