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
       emit ReceivedRequest(qStringBuffer, clientSocket);

       // Empty buffer
       memset(&buffer, '\0', BUFSIZE);
    }
}

void ClientServiceWorker::ProcessRequest(QString request, int socket) {
    if (request.left(4) == "song") {
        qDebug() << "I GOT A SONG";
        //QString songName = request.right(4);
        QString songName = "../Demo/Party_In_The_USA-Miley_Cyrus.wav";

        // get file and send to client
        SendFileToClient(songName, socket);
    }

    emit ProcessedRequest();
}

void ClientServiceWorker::SendFileToClient(QString songName, SOCKET cSocket) {
    char *fileName;
    // song files 46-51 MB
    char buffer[600000] = { 0 };
    fileName = buffer;
    char * ff;
    int bytesRead;
    qDebug() << songName;
    FILE * file;

    /*
    int index = 0;
    //find the corresponding song songName with filepath
    for(QStringList::iterator it = playlist.begin(); it != playlist.end(); ++it, index++){
        QString current = *it;
        if(QString::compare(current,songName,Qt::CaseSensitive) == 0){
            songName = playlistWithPath.at(index);
            break;
        }
    }

    if(!(file = fopen(songName.toStdString().c_str(), "rb+"))){
        char buffer2[2];
        strcpy(buffer2, ERROR_BIT);
        //char * buffer2 = ERROR_BIT;
        send(cSocket, buffer2, DATA_BUFSIZE, 0);
        return;
    }

    char success[] = "M";
    sendDataTCP(m_socket, success);
    while((bytesRead = fread(buffer, sizeof(char), FILEMAX, file))){
            if(bytesRead != FILEMAX){
            send(m_socket, buffer, bytesRead, 1000);

            break;
        }
        n.WSAS(m_socket, buffer, 60000, 1000);
    }
    */
}
