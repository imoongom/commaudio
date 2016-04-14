#ifndef TCPCONTROLWORKER_H
#define TCPCONTROLWORKER_H

#include <QObject>
#include <QMap>
#include "../Global.h"
#include "Server.h"
#include "ClientServiceWorker.h"

class TCPControlWorker : public QObject {
    Q_OBJECT
public:
    TCPControlWorker() {}
    ~TCPControlWorker() {
        listeningSocket = 0;
        CloseSocket();
    }

    boolean SendOne(LPSOCKET_INFORMATION socketInformation, char * message);
    boolean SendAll(char *message, LPDWORD lpNumberOfBytesSent);
    QString CreateClientMapMessage();
    void SendFileToClient(QString title, SOCKET m_socket);

signals:
    void SignalInitSocket(int port);
    void ClosedSocket();
    void AcceptedClient(QString ipAddr, int socket);
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

    QMap<int, QString> connectedClients;


};

#endif // TCPCONTROLWORKER_H
