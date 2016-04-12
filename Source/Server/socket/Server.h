#ifndef SERVER_H
#define SERVER_H
#include <iostream>
#include "../global.h"

// Virtual base class to be inherited for TCP and UDP sending
class Server {
public:
    Server() {}
    ~Server() {}

    virtual boolean InitSocket(int port) = 0;
    virtual boolean SendOne(LPSOCKET_INFORMATION SockInfo, char * message) = 0;
    virtual boolean SendAll(char *message, LPDWORD lpNumberOfBytesSent) = 0;
    virtual void CloseSocket() = 0;

protected:
    SOCKET_INFORMATION socketInformation;
    SOCKADDR_IN localAddr;
    WSADATA wsaData;
    DWORD flags = 0;
    int timeToLive = 1;
};


#endif // SERVER_H
