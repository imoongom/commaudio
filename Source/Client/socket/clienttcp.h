#ifndef CLIENTTCP_H
#define CLIENTTCP_H
#include <winsock2.h>
#include <QObject>

#define TCP_DEFAULT_HOST "10.1.0.64"
#define TCP_DEFAULT_PORT 7000
#define BUFSIZE     1024

class ClientTCP : public QObject
{
    Q_OBJECT
public:
    explicit ClientTCP(std::string  host = TCP_DEFAULT_HOST,
                       int portNum = TCP_DEFAULT_PORT,QObject *parent = 0);


    //method
    static DWORD WINAPI TCPThreadConnect(void * Param);
    boolean TCPConnect();
    void createThread();
    void TCPRecv();
    void TCPSend(char * message);
    void TCPSend2(char * message);
    void Send(int type, char* name, char* message);
    void CloseTCP();

    //varible
    char tcp_host_addr[BUFSIZE];
    int tcp_port;

    SOCKET tcpSock;


    struct hostent *host;

signals:

public slots:
};

#endif // CLIENTTCP_H
