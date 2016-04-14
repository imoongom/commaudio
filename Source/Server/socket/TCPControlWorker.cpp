#include <iostream>
#include <QApplication>
#include <QDebug>
#include "TCPControlWorker.h"

/* Initialize Windows socket */
void TCPControlWorker::InitSocket(int port) {
    QString acceptedClientIp;

    qDebug() << "In TCPControlWorker::InitSocket()";
    if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0 ) { //No useable DLL
        qDebug() << "DLL not found!\n";
        return;
    }

    // Create a stream socket
    if ((listeningSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        qDebug() << "Can't create a socket";
        return;
    }

    // Initialize and set up the address structure
    memset((char *)&serverAddr, 0, sizeof(struct sockaddr_in));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); // Accept connections from any client

    // Bind an address to the socket
    if (bind(listeningSocket, (PSOCKADDR)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        qDebug() << "TCP can't bind name to socket: " << WSAGetLastError();
        return;
    }

    qDebug() << "TCP connection established on socket: " << listeningSocket<< " port: " << port;
    qDebug() << "Socket Initialized. Listening for TCP connections...";

    // Listen for connections
    // queue up to 5 connect requests
    if (listen(listeningSocket, 5) == SOCKET_ERROR) {
        qDebug() << "Error on listen: " << WSAGetLastError();
        return;
    }

    // Accept clients
    while (tcpConnected) {
        clientLen= sizeof(clientAddr);

        if ((acceptedClientSocket = accept (listeningSocket, (struct sockaddr *)&clientAddr, &clientLen)) == -1) {
            qDebug() << "Can't accept client, " << WSAGetLastError();
            return;
        }

        qDebug() << "Accepted client" << inet_ntoa(clientAddr.sin_addr);

        // Add client to map
        acceptedClientIp = inet_ntoa(clientAddr.sin_addr);
        qDebug() << "Inserting into map " << acceptedClientSocket << " : " << acceptedClientIp;
        connectedClients.insert(acceptedClientSocket, acceptedClientIp);

        // Send client map to all clients
        QString qMessage = CreateClientMapMessage();
        char* sendMessage = qMessage.toUtf8().data();
        for (QMap<int, QString>::iterator it = connectedClients.begin(); it != connectedClients.end(); ++it) {
            qDebug() << "Sending " << sendMessage << " to socket" << it.key();
            send(it.key(), sendMessage, BUFSIZE, 0);
        }

        emit AcceptedClient(acceptedClientIp, acceptedClientSocket);
    }
    emit finished();
}

/* Send to a client using completion routine */
boolean TCPControlWorker::SendOne(LPSOCKET_INFORMATION socketInformation, char * message) {
    ZeroMemory(&socketInformation->Overlapped, sizeof(WSAOVERLAPPED));
    socketInformation->Overlapped.hEvent = WSACreateEvent();

    if (WSASend(socketInformation->Socket, &(socketInformation->DataBuf), 1, &socketInformation->BytesSEND,
        0, &(socketInformation->Overlapped), NULL) == SOCKET_ERROR) {
        if (WSAGetLastError() != ERROR_IO_PENDING) {
            qDebug() << "TCPControlWorker::SendToClient() WSASend() failed with: " << WSAGetLastError();
            return false;
        }

        if (WSAWaitForMultipleEvents(1, &socketInformation->Overlapped.hEvent, FALSE, INFINITE, FALSE) == WAIT_TIMEOUT) {
            qDebug() << "TCPControlWorker::SendToClient timed out";
            return false;
        }
    }
    if (!WSAGetOverlappedResult(socketInformation->Socket, &(socketInformation->Overlapped),
                               &socketInformation->BytesSEND, FALSE, &flags)) {
        qDebug() << "TCPControlWorker::SendToClient overlappedresult error" << WSAGetLastError();
        return false;
    }
    return true;
}

/* Send without completion routine */
boolean TCPControlWorker::SendOne2(LPSOCKET_INFORMATION socketInformation, char* message) {
    if (send(socketInformation->Socket, message, BUFSIZE, 0) == -1) {
        qDebug()<<"Send failed";
        return false;
    }
    return true;
}

/* Send to all connected clients */
boolean TCPControlWorker::SendAll(char *message, LPDWORD lpNumberOfBytesSent) {
    QMap<int, QString>::iterator i;
    for(i = connectedClients.begin(); i != connectedClients.end(); i++) {
        int socket = i.key();
        if(send(socket, message, DATA_BUFSIZE, 0) == -1) {
            qDebug() << "Broadcast() failed for client IP: " << i.key() << " error: " << WSAGetLastError();
            return false;
        }
    }
    return true;
}

void TCPControlWorker::CloseSocket(){
    qDebug() << "Closing TCP socket...";
    closesocket(listeningSocket);
    WSACleanup();
    emit ClosedSocket();
    qDebug() << "Closed TCP socket.";
}

QString TCPControlWorker::CreateClientMapMessage() {
    QString result = "";
    int i = 1;

    for (QMap<int, QString>::iterator it = connectedClients.begin(); it != connectedClients.end(); ++it, ++i) {
        result += "<" + it.value();
        qDebug() << "result: " << result;
    }

    return result;    
}
