#ifndef SERVERUDP_H
#define SERVERUDP_H
#include <winsock2.h>
#include <ws2tcpip.h>
#include "server.h"

class ServerUDP : public Server {
public:
    ServerUDP() {}
    ~ServerUDP() {}

    boolean InitSocket(int port);
    boolean MulticastSettings(const char * name);
    boolean SendOne(LPSOCKET_INFORMATION SockInfo, char * message);
    boolean SendAll(char *message, LPDWORD lpNumberOfBytesSent);
    boolean InitData();
    void CloseSocket();


    SOCKET hSocket;
    char hostAddr[MAXADDRSTR];
    u_long lhostAddr;
    u_short nPort;

private:
    SOCKADDR_IN destIpAddr;
    struct ip_mreq multicastAddr;
};

#endif // SERVERUDP_H
