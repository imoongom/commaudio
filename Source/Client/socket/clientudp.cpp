#include "clientudp.h"
#include "../global.h"
#include <QDebug>


struct CBuffer CBuf;

int ClientUDP::Start() {
    int nRet;
    BOOL  fFlag;
    SOCKADDR_IN locAddr;



    WSADATA stWSAData;

    nPort = TIMECAST_PORT;

       /* Init WinSock */
    nRet = WSAStartup(MAKEWORD(2, 2), &stWSAData);
    if (nRet != 0) {
        qDebug("WSAStartup failed: %d\r\n", nRet);
        exit(1);
    }


    /* Get a datagram socket */
    hSocket = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, 0, 0, WSA_FLAG_OVERLAPPED);
    if (hSocket == INVALID_SOCKET) {
        qDebug("socket() failed, Err: %d\n", WSAGetLastError());
        WSACleanup();
        exit(1);
    }
    hSock = hSocket;
    qDebug("[OPENSOCKET]hSocket : %d\thSock: %d\n",hSocket, hSock);

    fFlag = TRUE;
    nRet = setsockopt(hSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&fFlag,
                        sizeof(fFlag));
    if (nRet == SOCKET_ERROR) {
        qDebug("setsockopt() SO_REUSEADDR failed, Err: %d\n",
            WSAGetLastError());
    }

    /* Name the socket (assign the local port number to receive on) */
    locAddr.sin_family = AF_INET;
    locAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    locAddr.sin_port = htons(nPort);
    nRet = bind(hSocket, (struct sockaddr*) &locAddr, sizeof(locAddr));
    if (nRet == SOCKET_ERROR) {
        qDebug("bind() port: %d failed, Err: %d\n", nPort,
            WSAGetLastError());
    }

    qDebug("[BIND]hSocket : %d\thSock: %d\n",hSocket, hSock);
}


void ClientUDP::multiSetup(){
    int nRet;
    qDebug("[multiSetup]hSocket : %d\thSock: %d\n",hSocket, hSock);

    /* Join the multicast group so we can receive from it */
    stMreq.imr_multiaddr.s_addr = inet_addr(hostAddr);
    stMreq.imr_interface.s_addr = INADDR_ANY;
    nRet = setsockopt(hSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&stMreq,
                    sizeof(stMreq));

    if (nRet == SOCKET_ERROR) {
        qDebug(
            "setsockopt() IP_ADD_MEMBERSHIP address %s failed, Err: %d\n",
            hostAddr, WSAGetLastError());
    }
    qDebug("  multicast group address: %s, port number: %d\n", hostAddr, nPort);


}
void ClientUDP::initData(){
    initBuffer(&CBuf);
 //   if((fstream = fopen("NeYo-SoSick.wav", "wb"))== NULL){
    if((fstream = fopen("nananafile.txt", "wb"))== NULL){
        qDebug() << "file open fail\n";
            return;
          }
}



void ClientUDP::writeFile(){
     qDebug("WriteFile: ");
     char buffer[CIRBUFSIZE];
        if(CBuf._count==0)
            return;
        read_buffer(&CBuf, &buffer);
        qDebug()<<"BUFFER READ : " << buffer;
        fwrite(buffer, sizeof(char), CIRBUFSIZE, fstream);

}


int ClientUDP::close() {
    int nRet;

    stMreq.imr_multiaddr.s_addr = inet_addr(hostAddr);
    stMreq.imr_interface.s_addr = INADDR_ANY;
    nRet = setsockopt(hSock, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char *)&stMreq,
        sizeof(stMreq));

    if (nRet == SOCKET_ERROR) {
        qDebug(
            "setsockopt() IP_DROP_MEMBERSHIP address %s failed, Err: %d\n",
            hostAddr, WSAGetLastError());
    }

    /* Close the socket */
    closesocket(hSock);
    fclose(fstream);

    /* Tell WinSock we're leaving */
    WSACleanup();

    return (0);
}


