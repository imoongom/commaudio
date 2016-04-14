#include <QApplication>
#include <QDebug>
#include "ClientServiceWorker.h"
#include "Global.h"

SOCKET tcpSock;

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
       emit ReceivedRequest(qStringBuffer);

       // Empty buffer
       memset(&buffer, '\0', BUFSIZE);
    }
}

void ClientServiceWorker::ProcessRequest(QString request) {
    if (request.left(4) == "song") {
        qDebug() << "I GOT A SONG";
        // get file and send to client
    }

    emit ProcessedRequest();
}
