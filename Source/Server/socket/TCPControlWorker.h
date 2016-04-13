#ifndef TCPCONTROLWORKER_H
#define TCPCONTROLWORKER_H
#include <QObject>
#include <QMap>
#include "../global.h"
#include "Server.h"

class TCPControlWorker : public QObject {
    Q_OBJECT
public:
    TCPControlWorker() {}
    ~TCPControlWorker() {
        listeningSocket = 0;
        CloseSocket();
    }

    boolean AcceptClient();
    boolean SendOne(LPSOCKET_INFORMATION socketInformation, char * message);
    boolean SendOne2(LPSOCKET_INFORMATION socketInformation, char* message);
    boolean SendAll(char *message, LPDWORD lpNumberOfBytesSent);

signals:
    void SignalInitSocket(int port);
    void ClosedSocket();
    void AcceptedClient(int socket);
    void finished();

public slots:
    void InitSocket(int port);
    void CloseSocket();

private:
    SOCKET listeningSocket, acceptedClientSocket;
    SOCKADDR_IN serverAddr, clientAddr;
    WSADATA WSAData;
    DWORD flags = 0;
    int clientLen;
    int numConnectedClients = 0;

    QMap<QString, int> connectedClients;
};

#endif // TCPCONTROLWORKER_H
