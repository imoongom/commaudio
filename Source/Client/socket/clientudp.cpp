#include "clientudp.h"
#include "../global.h"
#include <QDebug>

boolean ClientUDP::Start(SOCKET* sock, int port) {
    int nRet;
    BOOL  fFlag;
    SOCKADDR_IN locAddr;


    WSADATA stWSAData;

    nPort = port;

       /* Init WinSock */
    nRet = WSAStartup(MAKEWORD(2, 2), &stWSAData);
    if (nRet != 0) {
        qDebug("WSAStartup failed: %d\r\n", nRet);
        return false;
    }


    /* Get a datagram socket */
    hSocket = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, 0, 0, WSA_FLAG_OVERLAPPED);
    if (hSocket == INVALID_SOCKET) {
        qDebug("socket() failed, Err: %d\n", WSAGetLastError());
        WSACleanup();
        return false;
    }
    *sock = hSocket;
    qDebug("[UDPOPENSOCKET]hSocket : %d\thSock: %d\n",hSocket, *sock);

    fFlag = TRUE;
    nRet = setsockopt(hSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&fFlag,
                        sizeof(fFlag));
    if (nRet == SOCKET_ERROR) {
        qDebug("setsockopt() SO_REUSEADDR failed, Err: %d\n",
            WSAGetLastError());
        return false;
    }

    /* Name the socket (assign the local port number to receive on) */
    locAddr.sin_family = AF_INET;
    locAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    locAddr.sin_port = htons(nPort);
    nRet = bind(hSocket, (struct sockaddr*) &locAddr, sizeof(locAddr));
    if (nRet == SOCKET_ERROR) {
        qDebug("bind() port: %d failed, Err: %d\n", nPort,
            WSAGetLastError());
        return false;
    }

    qDebug("[UDPBIND]hSocket : %d\thSock: %d\n",hSocket, *sock);
    return true;
}


boolean ClientUDP::multiSetup(SOCKET *sock){
    int nRet;
    qDebug("[multiSetup]hSocket : %d\thSock: %d\n",hSocket, *sock);

    /* Join the multicast group so we can receive from it */
    stMreq.imr_multiaddr.s_addr = inet_addr(hostAddr);
    stMreq.imr_interface.s_addr = INADDR_ANY;
    nRet = setsockopt(hSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&stMreq,
                    sizeof(stMreq));

    if (nRet == SOCKET_ERROR) {
        qDebug(
            "setsockopt() IP_ADD_MEMBERSHIP address %s failed, Err: %d\n",
            hostAddr, WSAGetLastError());
        return false;
    }
    qDebug("  multicast group address: %s, port number: %d\n", hostAddr, nPort);
    return true;

}
/*
boolean ClientUDP::initData(){
//    initBuffer(&CBuf);
 //   if((fstream = fopen("NeYo-SoSick.wav", "wb"))== NULL){
    if((fstream = fopen("nananafile.txt", "wb"))== NULL){
        qDebug() << "file open fail\n";
        return false;
    }
    return true;
}
*/


void ClientUDP::sendVoice(char *ip){
        char temp[DATA_BUFSIZE];

        LPSOCKET_INFORMATION SIVoice;
        DWORD  SendBytes;
        SOCKET voiceTo;
        SOCKADDR_IN InetAddr;
        struct hostent *voiceHost;

        qDebug()<<"[VoiceSend] START";
        if ((SIVoice = (LPSOCKET_INFORMATION)GlobalAlloc(GPTR, sizeof(SOCKET_INFORMATION)))
            == NULL)
        {
            qDebug("[VoiceSend]GlobalAlloc() failed with error %d\n", WSAGetLastError());
            return;
        }


        if ((voiceTo = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, 0, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
        {
           qDebug("[VoiceSend]socket() failed with error %d\n", WSAGetLastError());
            return;
        }

        if ((voiceHost = gethostbyname(ip)) == NULL)
        {
            qDebug("[VoiceSend]Unable to resolve host name");
            return;
        }

        SIVoice->Socket = voiceTo;
        SIVoice->DataBuf.buf = temp;
        SIVoice->DataBuf.len = DATA_BUFSIZE;

        memset(&InetAddr, 0, sizeof(InetAddr));

        InetAddr.sin_family = AF_INET;
        InetAddr.sin_port = htons(UDP_DEFAULT_PORT);
        InetAddr.sin_addr.s_addr = *((unsigned long*)voiceHost->h_addr);


        while (_VoiceChat) {
            if(CBufOut._count ==0)
                continue;
            read_buffer(&CBufOut, temp);

            memset(&SIVoice->Overlapped, '\0', sizeof(SIVoice->Overlapped));
            SIVoice->Overlapped.hEvent = WSACreateEvent();

            memcpy(&SIVoice->DataBuf.buf, temp, DATA_BUFSIZE);
            if (WSASendTo(SIVoice->Socket, &(SIVoice->DataBuf), 1, &SendBytes, 0,
                (SOCKADDR *)&InetAddr, sizeof(InetAddr), &(SIVoice->Overlapped), NULL) == SOCKET_ERROR)
            {
                if (WSAGetLastError() != WSAEWOULDBLOCK && WSAGetLastError() != WSA_IO_PENDING)
                {
                    qDebug("[VoiceSend]WSASend() failed with error %d\n", WSAGetLastError());
                    return;
                }

            }

            if (WSAWaitForMultipleEvents(1, &SIVoice->Overlapped.hEvent, FALSE, 100, FALSE)== WAIT_TIMEOUT) {
                qDebug("[VoiceSend]UDP SEND TimeOut");
                //close();
                return;
            }

            SIVoice->BytesSEND += SendBytes;

        }

        free(SIVoice);

}

int ClientUDP::close() {
    int nRet;

    stMreq.imr_multiaddr.s_addr = inet_addr(hostAddr);
    stMreq.imr_interface.s_addr = INADDR_ANY;
    nRet = setsockopt(hSocket, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char *)&stMreq,
        sizeof(stMreq));

    if (nRet == SOCKET_ERROR) {
        qDebug(
            "setsockopt() IP_DROP_MEMBERSHIP address %s failed, Err: %d\n",
            hostAddr, WSAGetLastError());
    }
    clean_buffer(&CBuf);
    /* Close the socket */
    closesocket(hSocket);
    fclose(fstream);
    _MULTIconnectOn = false;
    /* Tell WinSock we're leaving */
    WSACleanup();

    return (0);
}


