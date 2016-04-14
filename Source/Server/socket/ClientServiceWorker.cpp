#include <QApplication>
#include <QDebug>
#include "ClientServiceWorker.h"
#include "Global.h"
#include "MainWindow.h"

/*---------------------------------------------------------------------------------------
--  SOURCE FILE:    ClientServiceWorker.cpp
--  PROGRAM:        COMP 4985 : Comm Audio
--  FUNCTIONS:
--          void ListenForRequests()
--          void ProcessRequest(QString request, int socket)
--          void SendFileToClient(QString songName, SOCKET cSocket) 
--  DATE:           April 9, 2016
--  REVISIONS:      N/A
--  DESIGNERS:      Krystle Bulalakaw
--  PROGRAMMER:     Krystle Bulalakaw
--  NOTES:
--  The worker class for a thread that handles client requests sent over a TCP 
--  connection. The client may send requests to play a specific song, download a file,
--  or enable P2P voice communication.
---------------------------------------------------------------------------------------*/

SOCKET tcpSock;

/*-----------------------------------------------------------------------------------------------------
-- FUNCTION:    Listen for Requests
-- DATE:        April 14, 2016
-- REVISIONS:   N/A
-- DESIGNER:    Krystle Bulalakaw
-- PROGRAMMER:  Krystle Bulalakaw
-- RETURNS:     void
-- INTERFACE: MulticastSettings(const char * name)
--          const char *name - name of the socket
-- INTERFACE: 
-- Runs in separate thread to initialize a TCP socket and listen on the socket for messages from
-- the client signifying requests.
-------------------------------------------------------------------------------------------------------*/

void ClientServiceWorker::ListenForRequests() {
    qDebug() << "ClientServiceWorker::ListenForRequests()";

    LPSOCKET_INFORMATION SI;
    char buffer[BUFSIZE] = "";
    QString qStringBuffer;
    DWORD  SendBytes, RecvBytes;
    DWORD Flags = 0;

    if ((SI = (LPSOCKET_INFORMATION)GlobalAlloc(GPTR, sizeof(SOCKET_INFORMATION))) == NULL) {
       qDebug("SI GlobalAlloc() failed with error %d\n", WSAGetLastError());
       return;
    }

    // Initialize socket information
    SI->Socket = clientSocket;
    SI->Overlapped.hEvent = WSACreateEvent();
    SI->DataBuf.buf = buffer;
    SI->DataBuf.len = BUFSIZE;
    RecvBytes = sizeof(SOCKET_INFORMATION);

    while (1) {
       memset(&SI->Overlapped, '\0', sizeof(SI->Overlapped));
       SI->Overlapped.hEvent = WSACreateEvent();

       qDebug() << SI->Socket;

       if (WSARecv(SI->Socket, &(SI->DataBuf), 1, &RecvBytes, &Flags,&(SI->Overlapped), NULL) == SOCKET_ERROR) {
           if (WSAGetLastError() != WSAEWOULDBLOCK && WSAGetLastError() != WSA_IO_PENDING) {
               qDebug()<<"ClientServiceWorker:: WSARecv() failed with error " << WSAGetLastError();
               return;
           }
       }

       qDebug() << "TCP received : " << SI->DataBuf.buf;
       if ((WSAWaitForMultipleEvents(1, &SI->Overlapped.hEvent, FALSE, 5000, FALSE)) == WAIT_TIMEOUT) {
           qDebug("Read done");
           return;
       }

       // Process buffer
       qStringBuffer = buffer;
       emit ReceivedRequest(qStringBuffer, clientSocket);

       // Empty buffer
       memset(&buffer, '\0', BUFSIZE);
    }
}

/*-----------------------------------------------------------------------------------------------------
-- FUNCTION:    ProcessRequests
-- DATE:        April 14, 2016
-- REVISIONS:   N/A
-- DESIGNER:    Krystle Bulalakaw
-- PROGRAMMER:  Krystle Bulalakaw
-- RETURNS:     void
-- INTERFACE:   ProcessRequest(QString request, int socket)
--                QString request - the request message received
--                int socket - socket of the sender
-- INTERFACE: 
-- Parses the message and handles the request.
-------------------------------------------------------------------------------------------------------*/
void ClientServiceWorker::ProcessRequest(QString request, int socket) {
    if (request.left(4) == "song") {
        qDebug() << "I GOT A SONG";
        QString songName = request.mid(4);
        //QString songName = "../Demo/Party_In_The_USA-Miley_Cyrus.wav";

        // get file and send to client
        SendFileToClient(songName, socket);
    }

    emit ProcessedRequest();
}

/*-----------------------------------------------------------------------------------------------------
-- FUNCTION:    SendFileToClient
-- DATE:        April 14, 2016
-- REVISIONS:   N/A
-- DESIGNER:    Krystle Bulalakaw
-- PROGRAMMER:  Krystle Bulalakaw
-- RETURNS:     void
-- INTERFACE:   SendFileToClient(QString songName, SOCKET cSocket) 
--                QString songName - requested song name
--                SOCKET cSocket - socket of the sender
-- INTERFACE: 
-- Opens a file and sends its contents to the client.
-------------------------------------------------------------------------------------------------------*/
void ClientServiceWorker::SendFileToClient(QString songName, SOCKET cSocket) {
    qDebug() << "ClientServiceWorker::SendFileToClient";
    char *fileName;
    // song files 46-51 MB
    int maxFileSize = 60000;
    char buffer[maxFileSize] = { 0 };
    fileName = buffer;
    char * ff;
    int bytesRead;
    songName = "../Demo/" + songName;
    qDebug() << "songName : " << songName;
    FILE * file;

    if(!(file = fopen(songName.toStdString().c_str(), "rb+"))){
        qDebug() << "Unable to open file";
        return;
    }

    send(cSocket, "HAJHAHA!", 10, 0);

    while((bytesRead = fread(buffer, sizeof(char), maxFileSize, file))){
        send(cSocket, buffer, bytesRead, 1000);
        qDebug() << "bytesRead " << bytesRead;
    }

    qDebug() << "send done";
}
