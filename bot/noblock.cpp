 
#include <fcntl.h> // Pour fcntl

void Bot::m_connectToServer()
{
    struct sockaddr_in socketAdd;

    socketAdd.sin_family = AF_INET;
    socketAdd.sin_port = htons(this->m_port);
    socketAdd.sin_addr.s_addr = inet_addr(this->m_serAddress.c_str());

    this->m_serSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->m_serSocket == -1)
        throw std::runtime_error("Error: Socket failed");

    if (connect(this->m_serSocket, (struct sockaddr*)&socketAdd, (socklen_t)(sizeof(socketAdd))) != 0)
        throw std::runtime_error("Error: Cannot connect to server");

    // Configurer le socket en mode non bloquant
    int flags = fcntl(this->m_serSocket, F_GETFL, 0);
    if (flags == -1 || fcntl(this->m_serSocket, F_SETFL, flags | O_NONBLOCK) == -1)
        throw std::runtime_error("Error: Failed to set non-blocking mode");
}

int Bot::m_run()
{
    char buffer[513];

    while (!this->m_signal)
    {
        std::cout << "signal is " << (this->m_signal ? "true" : "false") << std::endl;
        memset(buffer, 0, sizeof(buffer));
        int bytes = recv(this->m_serSocket, buffer, sizeof(buffer), 0);

        if (bytes < 0)
        {
            if (errno == EWOULDBLOCK || errno == EAGAIN)
            {
                // Pas de données disponibles, continuer à vérifier m_signal
                usleep(100000); // Facultatif : ajoutez un léger délai pour éviter de saturer le CPU
                continue;
            }
            else
            {
                // Une autre erreur est survenue
                throw std::runtime_error("Error: recv failed");
            }
        }
        else if (bytes == 0)
        {
            // Le serveur a fermé la connexion
            std::cerr << "Server closed connection" << std::endl;
            break;
        }

        buffer[bytes] = '\0';
        std::string message(buffer);
        std::cout << ">> " << message << std::endl;

        if (message.find("376") != std::string::npos)
        {
            speak("JOIN " + this->m_channel + "\r\n");
            speak("PRIVMSG " + this->m_channel + " :Hi everyone i'm an IRC File Transfer Bot\r\n");
            speak("PRIVMSG " + this->m_channel + " :You can ask me the list of transferable files with the command 'PRIVMSG FileHandlerBot !list'\r\n");
            speak("PRIVMSG " + this->m_channel + " :I can transfer you the file you want with the command 'PRIVMSG FileHandlerBot !send [filename]'\r\n");
        }

        if (message.find("PRIVMSG") != std::string::npos)
            m_handlePrivMsg(message);
    }

    return quit();
}
