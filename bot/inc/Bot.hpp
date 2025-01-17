#ifndef BOT_HPP
# define BOT_HPP

#include <fstream>
# include <string>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <vector>
# define DCCTIMEOUT 60
# define BANDIC "botShareDirectory/banDic.txt"
# define FILEDIR "botShareDirectory"

class Bot {
public:
    Bot(std::string serAdd, std::string channel, std::string password, int port);
    ~Bot();

    std::string 				getNick() const;
    std::string 				getUsername() const;
    std::string 				getRealname() const;
    int 						init();
    int 						quit();
    void 						speak(const std::string & msg);
	static void					signalHandler(int signum);

private:
    static bool 				m_signal;
    std::string 				m_serAddress;
    std::string 				m_name;
	std::string					m_channel;
    std::string 				m_password;
	std::vector<std::string>	m_vlist;
	std::vector<std::string>	m_vbanDic;
    int 						m_port;
    int 						m_serSocket;
	void						m_authenticate();
	void						m_connectToServer();
	void						m_helloWorld();
	void						m_createList();
	void						m_createDictionary();
    int							m_run();
	uint32_t					m_getLocalIpInt() const;
	void						m_createSocket(struct sockaddr_in & serverAddr, int & serverSock);
	void						m_acceptAndSend(int & serverSock, int & clientSock, std::ifstream & file);
	bool						isInList(const std::string & filename);

	// 				HANDLE CMD
	void						m_sendFileData(int & clientSock, std::ifstream & file);
	void						m_handlePrivMsg(std::string & message);
	void						m_handleList(std::vector<std::string> & tokens);
    void						m_handleSendFile(std::vector<std::string> & tokens);
	void						m_handleRefresh(std::vector<std::string> & tokens);
	void						m_handleKick(std::vector<std::string> & tokens);

	// 				UTILS
};

std	::string					trimNewLines(const std::string & str);
std::vector<std::string>		vsplit(const std::string & str, char delimiter);
size_t							getFileSize	(std::ifstream & file);
std::string						toLowerStr(const std::string & str);

#	endif
