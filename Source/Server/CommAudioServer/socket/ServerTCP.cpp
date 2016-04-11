#include <iostream>
#include <string>
#include<QDebug>
#include "ServerTCP.h"
using namespace std;

#define TCP_DEFAULT_HOST "127.0.0.1"
#define TCP_DEFAULT_PORT 7000
int	n, ns, bytes_to_read;
int	client_len, err;
SOCKET sd, new_sd;
struct	sockaddr_in client;
char	*bp, buf[BUFSIZE];
WSADATA WSAData;
WORD wVersionRequested;

/* Initialize Windows socket */
boolean ServerTCP::InitSocket(int port) {
    wVersionRequested = MAKEWORD( 2, 2 );
    err = WSAStartup( wVersionRequested, &WSAData );
    if ( err != 0 ) //No useable DLL
    {
        qDebug() << "DLL not found!\n";
        return false;
    }

    // Create a stream socket
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        qDebug() << "Can't create a socket";
        return false;
    }

    // Initialize and set up the address structure
    memset((char *)&localAddr, 0, sizeof(struct sockaddr_in));
    localAddr.sin_family = AF_INET;
    localAddr.sin_port = htons(port);
    localAddr.sin_addr.s_addr = htonl(INADDR_ANY); // Accept connections from any client

    // Bind an address to the socket
    if (bind(sd, (struct sockaddr *)&localAddr, sizeof(localAddr)) == -1) {
        qDebug() << "TCP can't bind name to socket";
        return false;
    }

    // Listen for connections
    // queue up to 1 connect request
    listen(sd, 1);

    return true;
}

/* Accept a client and add them to the map */
boolean ServerTCP::AcceptClient() {
    if ((new_sd = accept (sd, (struct sockaddr *)&client, &client_len)) == -1) {
        qDebug() << "Can't accept client, " << WSAGetLastError();
        return false;
    }

    qDebug() << "Accepted client" << inet_ntoa(client.sin_addr);
    string clientIp = inet_ntoa(client.sin_addr);
    return true;
}

/* Send to a client using completion routine */
boolean ServerTCP::SendOne(LPSOCKET_INFORMATION socketInformation, char * message) {
    ZeroMemory(&socketInformation->Overlapped, sizeof(WSAOVERLAPPED));
    socketInformation->Overlapped.hEvent = WSACreateEvent();

    if (WSASend(socketInformation->Socket, &(socketInformation->DataBuf), 1, &socketInformation->BytesSEND,
        0, &(socketInformation->Overlapped), NULL) == SOCKET_ERROR) {
        if (WSAGetLastError() != ERROR_IO_PENDING) {
            qDebug() << "ServerTCP::SendToClient() WSASend() failed with: " << WSAGetLastError();
            return false;
        }

        if (WSAWaitForMultipleEvents(1, &socketInformation->Overlapped.hEvent, FALSE, INFINITE, FALSE) == WAIT_TIMEOUT) {
            qDebug() << "ServerTCP::SendToClient timed out";
            return false;
        }
    }
    if (!WSAGetOverlappedResult(socketInformation->Socket, &(socketInformation->Overlapped),
                               &socketInformation->BytesSEND, FALSE, &flags)) {
        qDebug() << "ServerTCP::SendToClient overlappedresult error" << WSAGetLastError();
        return false;
    }
    return true;
}

/* Send without completion routine */
boolean ServerTCP::SendOne2(LPSOCKET_INFORMATION socketInformation, char* message) {
    if (send(socketInformation->Socket, message, BUFSIZE, 0) == -1) {
        qDebug()<<"Send failed";
        return false;
    }
    return true;
}

/* Send to all connected clients */
boolean ServerTCP::SendAll(char *message, LPDWORD lpNumberOfBytesSent) {
    ClientInfo clientInfo;
    for(const auto &pair : mapClientInfo)
    {
        clientInfo = pair.second;
        if(send(clientInfo.socketInformation.Socket, message, DATA_BUFSIZE, 0) == -1)
        {
            std::cerr << "Broadcast() failed for player id: " << pair.first << std::endl;
            std::cerr << "errno: " << WSAGetLastError() << std::endl;
            return false;
        }
    }
    return true;
}

