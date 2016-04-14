#include "clienttcp.h"
#include "../global.h"
#include <QDebug>
ClientTCP::ClientTCP(std::string  host, int portNum, QObject *parent) : QObject(parent)
{
    strcpy(tcp_host_addr, host.c_str());
    tcp_port = portNum;

    TCPConnect();
}



DWORD WINAPI ClientTCP::TCPThreadConnect(void * Param) {
    ClientTCP* cthis = (ClientTCP*)Param;
    cthis->TCPRecv();
    return NULL;
}


boolean ClientTCP::TCPConnect() {

    SOCKADDR_IN InetAddr;

    WSADATA stWSAData;
    BOOL  fFlag;
    DWORD nRet;

    /* Init WinSock */
    nRet = WSAStartup(MAKEWORD(2, 2), &stWSAData);
    if (nRet != 0) {
        qDebug("WSAStartup failed: %d\r\n", nRet);
        return false;
    }

    // Create the socket
    if ((tcpSock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
    {
        qDebug("Cannot create socket");
        return false;
    }
    fFlag = TRUE;
    if (setsockopt(tcpSock, SOL_SOCKET, SO_REUSEADDR, (char *)&fFlag,
        sizeof(fFlag)) == SOCKET_ERROR) {
        qDebug("setsockopt() SO_REUSEADDR failed, Err: %d\n",
            WSAGetLastError());
        return false;
    }

    if ((host = gethostbyname(tcp_host_addr)) == NULL)
    {
        qDebug("Unable to resolve host name");
        return false;
    }

    memset(&InetAddr, 0, sizeof(SOCKADDR_IN));
    InetAddr.sin_family = AF_INET;
    InetAddr.sin_port = htons(tcp_port);
    InetAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);

    // Connecting to the server
    if (WSAConnect(tcpSock, (SOCKADDR*)&InetAddr, sizeof(InetAddr), 0, 0, 0, NULL) == SOCKET_ERROR)
    {
        qDebug("WSAConnect() port: %d failed, Err: %d\n", tcp_port,
            WSAGetLastError());
        return false;
    }
    return true;
}

void ClientTCP::TCPcreateThread() {
    qDebug("### TCP CLIENT create Thread\n");
    HANDLE ThreadHandle;
    if ((ThreadHandle = CreateThread(NULL, 0, TCPThreadConnect, this, 0, NULL)) == NULL)
    {
        qDebug("CreateThread failed with error %d\n", GetLastError());
        return;
    }
    WaitForSingleObject(ThreadHandle, 5000);
   // qDebug("### TCP CLIENT thread CREATED!!!!!!!!\n");
}

// TCP Receive routine(Thread function)
void ClientTCP::TCPRecv() {
    qDebug() << "TCPRecv() waiting for data";
    LPSOCKET_INFORMATION SI;
    char temp[BUFSIZE] = "";
    char temp2[BUFSIZE] = "";
    DWORD  SendBytes, RecvBytes;
    DWORD Flags = 0;
    int i = 0;
    if ((SI = (LPSOCKET_INFORMATION)GlobalAlloc(GPTR, sizeof(SOCKET_INFORMATION)))
        == NULL)
    {
        qDebug("GlobalAlloc() failed with error %d\n", WSAGetLastError());
        return;
    }
    SI->Socket = tcpSock;
    SI->Overlapped.hEvent = WSACreateEvent();
    SI->DataBuf.buf = temp;
    SI->DataBuf.len = BUFSIZE;
    RecvBytes = sizeof(SOCKET_INFORMATION);
    while (1) {
        memset(&SI->Overlapped, '\0', sizeof(SI->Overlapped));
        SI->Overlapped.hEvent = WSACreateEvent();

        if (WSARecv(SI->Socket, &(SI->DataBuf), 1, &RecvBytes, &Flags,
            &(SI->Overlapped), NULL) == SOCKET_ERROR)
        {
            qDebug() << "bytesrecv " << RecvBytes;
            if (WSAGetLastError() != WSAEWOULDBLOCK && WSAGetLastError() != WSA_IO_PENDING)
            {
                qDebug()<<"WSARecv() failed with error " << WSAGetLastError()<<"\n";

                return;
            }

        }
            qDebug("RECV : %s\n", SI->DataBuf.buf);
        if ((WSAWaitForMultipleEvents(1, &SI->Overlapped.hEvent, FALSE, 5000, FALSE)) == WAIT_TIMEOUT) {
            qDebug("READ DOME");
            return;
        }


        i++;
           // wsprintf(temp2, "[sendback]%s\nHELLO THIS IS TCP  CLIENT[%d] SEND!!\n", (SI->DataBuf.buf).c_str(), i);
        qDebug()<<"TCPSEND PASS";
        //TCPSend(temp);
        qDebug()<<"EMPTY BUFFER";
        memset(&temp, '\0', BUFSIZE);
    }
}

//TCP SEND FUNCTION using completion routine
void ClientTCP::TCPSend(char * message) {
    qDebug()<<"TCP SEMD CALLED";
    LPSOCKET_INFORMATION SI;
    DWORD  SendBytes;
    int i = 0;
    if ((SI = (LPSOCKET_INFORMATION)GlobalAlloc(GPTR, sizeof(SOCKET_INFORMATION)))
        == NULL)
    {
        qDebug("GlobalAlloc() failed with error %d\n", WSAGetLastError());
        return;
    }
    SI->Socket = tcpSock;

    SI->DataBuf.buf = message;
    SI->DataBuf.len = BUFSIZE;


    memset(&SI->Overlapped, '\0', sizeof(SI->Overlapped));
    SI->Overlapped.hEvent = WSACreateEvent();

    if (WSASend(SI->Socket, &(SI->DataBuf), 1, &SendBytes, 0,
        &(SI->Overlapped), NULL) == SOCKET_ERROR)
    {
        if (WSAGetLastError() != WSAEWOULDBLOCK && WSAGetLastError() != WSA_IO_PENDING)
        {
            qDebug("WSASend() failed with error %d\n", WSAGetLastError());
            return;
        }

    }
    if ((WSAWaitForMultipleEvents(1, &SI->Overlapped.hEvent, FALSE, 100, FALSE)) == WAIT_TIMEOUT) {
        qDebug("READ DOME");
        return;
    }
    qDebug()<<"SEND SUCCESS TO " << tcpSock << " " << SI->DataBuf.buf;
  //  memset(&SI->DataBuf.buf, '\0', BUFSIZE);
        SI->BytesSEND += SendBytes;

}
//TCP SEND FUNCTION without using completion routine
void ClientTCP::TCPSend2(char * message) {

    if (send(tcpSock, message, BUFSIZE, 0) == -1)
    {
        qDebug()<<"SEND FAIL";
        return;
    }
}


void ClientTCP::Send(int type, char* name, char* message){
    TCP_MESG sendForm;

    sendForm.type = type;
    sendForm._end = false;

    strcpy(sendForm.name, name);
    strcpy(sendForm.data, message);
    //change to structure
    TCPSend(message);
}

void ClientTCP::CloseTCP(){
    _TCPconnectOn = false;
    closesocket(tcpSock);
    return;
}
