#include <iostream>
#include <QApplication>
#include <QDebug>
#include "TCPControlWorker.h"

/*---------------------------------------------------------------------------------------
--  SOURCE FILE:    TCPControlWorker.cpp
--  PROGRAM:        COMP 4985 : Comm Audio
--  FUNCTIONS:
--          void InitSocket(int port)
--          void SendOne(LPSOCKET_INFORMATION socketInformation, char * message)
--          void CloseSocket()

--  DATE:           April 9, 2016
--  REVISIONS:      N/A
--  DESIGNERS:      Krystle Bulalakaw
--  PROGRAMMER:     Krystle Bulalakaw
--  NOTES:
--  The worker class for a thread that handles initializing a TCP socket, listening
--  for new connections, and keeping track of connected clients.
---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------
-- FUNCTION:    InitSocket
-- DATE:        April 14, 2016
-- REVISIONS:   N/A
-- DESIGNER:    Krystle Bulalakaw
-- PROGRAMMER:  Krystle Bulalakaw
-- RETURNS:     void
-- INTERFACE:   InitSocket(int port)
-- NOTES:
-- Initializes the socket, sets up the address structure, then binds the address to the socket.
-- Then listens for up to 5 connections at a time, which will each have a service worker thread 
-- created for it.
-------------------------------------------------------------------------------------------------------*/
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
        qDebug() << "qMessage: " << qMessage;
        //char* sendMessage = qMessage.toUtf8().data();
        char *temp = new char[ qMessage.length() + 1 ]; // + 1 for zero in the end of string
        memcpy(temp, qMessage.toUtf8().constData(), qMessage.length() + 1);

        qDebug() << "sendmessage: " << temp;
        for (QMap<int, QString>::iterator it = connectedClients.begin(); it != connectedClients.end(); ++it) {
            qDebug() << "Sending " << temp << " to socket" << it.key();
            send(it.key(), temp, BUFSIZE, 0);
        }

        delete []temp;

        emit AcceptedClient(acceptedClientIp, acceptedClientSocket);
    }
    emit finished();
}

/*-----------------------------------------------------------------------------------------------------
-- FUNCTION:    InitSocket
-- DATE:        April 14, 2016
-- REVISIONS:   N/A
-- DESIGNER:    Krystle Bulalakaw
-- PROGRAMMER:  Krystle Bulalakaw
-- RETURNS:     boolean
-- INTERFACE:   SendOne(LPSOCKET_INFORMATION socketInformation, char * message)
--                  LPSOCKET_INFORMATION socketInformation - socket information struct of destination
--                  char* message - message to send
-- NOTES:
-- Sends to a client on a TCP socket by completion routine. 
-------------------------------------------------------------------------------------------------------*/
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

/*-----------------------------------------------------------------------------------------------------
-- FUNCTION:    SendAll
-- DATE:        April 14, 2016
-- REVISIONS:   N/A
-- DESIGNER:    Krystle Bulalakaw
-- PROGRAMMER:  Krystle Bulalakaw
-- RETURNS:     boolean
-- INTERFACE:   SendAll(char *message, LPDWORD lpNumberOfBytesSent)
--                  char* message - message to send
--                  LPDWORD lpNumberofBytesSent - number of bytes sent
-- NOTES:
-- Sends to all connected clients without completion routine.
-------------------------------------------------------------------------------------------------------*/
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

/*-----------------------------------------------------------------------------------------------------
-- FUNCTION:    CloseSocket
-- DATE:        April 14, 2016
-- REVISIONS:   N/A
-- DESIGNER:    Krystle Bulalakaw
-- PROGRAMMER:  Krystle Bulalakaw
-- RETURNS:     void
-- INTERFACE:   CloseSocket()
-- NOTES:
-- Closes the TCP socket.
-------------------------------------------------------------------------------------------------------*/
void TCPControlWorker::CloseSocket(){
    qDebug() << "Closing TCP socket...";
    closesocket(listeningSocket);
    WSACleanup();
    emit ClosedSocket();
    qDebug() << "Closed TCP socket.";
}

/*-----------------------------------------------------------------------------------------------------
-- FUNCTION:    CreateClientMapMessage
-- DATE:        April 14, 2016
-- REVISIONS:   N/A
-- DESIGNER:    Krystle Bulalakaw
-- PROGRAMMER:  Krystle Bulalakaw
-- RETURNS:     QString
-- INTERFACE:   CreateClientMapMessage()
-- NOTES:
-- Creates a string of all connected client's IP delimited by a symbol.
-------------------------------------------------------------------------------------------------------*/
QString TCPControlWorker::CreateClientMapMessage() {
    QString result = "";
    int i = 1;

    for (QMap<int, QString>::iterator it = connectedClients.begin(); it != connectedClients.end(); ++it, ++i) {
        result += "@" + it.value();
        qDebug() << "result: " << result;
    }

    return result;    
}
