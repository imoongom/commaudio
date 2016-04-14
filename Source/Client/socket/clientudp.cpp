#include "clientudp.h"
#include "../global.h"
#include "recording.h"

#include <QDebug>

extern Recording *gRecording;

boolean ClientUDP::Start(SOCKET* sock, int port) {
    int nRet;
    BOOL  fFlag;
    SOCKADDR_IN locAddr;


    WSADATA stWSAData;

    nPort = port;
    _type = UDP_CLIENT;

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

void ClientUDP::voiceStart(){
    emit voiceGo("127.0.0.1");
}

boolean ClientUDP::multiSetup(SOCKET *sock){
    int nRet;
    qDebug("[multiSetup]hSocket : %d\thSock: %d\n",hSocket, *sock);

    _type = MULTI_CLIENT;
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

void ClientUDP::udpConn(){

    DWORD  SendBytes;




    DWORD nRet;
    WSADATA stWSAData;
    char ip[15] = "192.168.0.15";
       /* Init WinSock */
    nRet = WSAStartup(MAKEWORD(2, 2), &stWSAData);
    if (nRet != 0) {
        qDebug("WSAStartup failed: %d\r\n", nRet);
        return ;
    }


    qDebug()<<"[VoiceSend] START" << ip;
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
    /*
    if ((voiceHost = gethostbyname("127.0.0.1")) == NULL)
    {
        qDebug("[VoiceSend]Unable to resolve host name");
        return;
    }
*/
    SIVoice->Socket = voiceTo;

    SIVoice->DataBuf.len = DATA_BUFSIZE;

    memset(&InetAddr, 0, sizeof(InetAddr));

    InetAddr.sin_family = AF_INET;
    InetAddr.sin_port = htons(1234);

   // memcpy((char *)&InetAddr.sin_addr.s_addr, voiceHost->h_addr, voiceHost->h_length);
    if((InetAddr.sin_addr.s_addr = inet_addr(ip))==INADDR_NONE)
    {
        qDebug() << "INVALID IP ";
    }

    qDebug()<<"[VoiceSend] SETUP FINISHED " << _VoiceChat << " " << voiceTo;
    emit connected();
}

void ClientUDP::sendVoice(){//char *ip){
    char temp[DATA_BUFSIZE];
    DWORD  SendBytes;

       while (_VoiceChat) {
           if(UDPbuf._count == 0)
               continue;//continue;

            qDebug() << "Voice Chat start" <<UDPbuf._count;
           SIVoice->DataBuf.buf = temp;

            qDebug()<<"VoiceChat SEND " << voiceTo;
            read_buffer(&UDPbuf, temp);


            memset(&SIVoice->Overlapped, '\0', sizeof(SIVoice->Overlapped));
            SIVoice->Overlapped.hEvent = WSACreateEvent();


            if (WSASendTo(SIVoice->Socket, &(SIVoice->DataBuf), 1, &SendBytes, 0,
                (SOCKADDR *)&InetAddr, sizeof(InetAddr), &(SIVoice->Overlapped), NULL) == SOCKET_ERROR)
            {
                if (WSAGetLastError() != WSAEWOULDBLOCK && WSAGetLastError() != WSA_IO_PENDING)
                {
                    qDebug("[VoiceSend]WSASendto() failed with error %d\n", WSAGetLastError());
                    return;
                }

            }

            if (WSAWaitForMultipleEvents(1, &SIVoice->Overlapped.hEvent, FALSE, 1000, FALSE)== WAIT_TIMEOUT) {
                qDebug("[VoiceSend]UDP SEND TimeOut");
                //close();
                return;
            }



        }
}

void ClientUDP::SendCapturedMicData(Recording *recording) {
    return;
    QByteArray qba;
    qint64 length = 0;
    char temp[DATA_BUFSIZE];

    gRecording = recording;

    while (_VoiceChat) {
        qba = recording->m_audioInputDevice->readAll();
        int nRet;
        if (qba.length() > 0) {
            nRet = sendto(voiceTo, qba, qba.length(), 0, (SOCKADDR *)&(InetAddr), sizeof(InetAddr));
           // qDebug() << "Sending captured mic data: " << qba;
        }
        if (nRet < 0) {
            qDebug() << "sendto failed: " << WSAGetLastError();
        }
    }
}

void ClientUDP::PlaybackVoiceData(char* data, DWORD len)
{
    qDebug() << "abc";
    //QByteArray* qba = new QByteArray(data, len);
    gRecording->m_audioOutputDevice->write(data, len);
}

int ClientUDP::UDPClose() {
    int nRet;

    switch(_type){
    case UDP_CLIENT:
        _UDPconnectOn = false;
        break;
    case MULTI_CLIENT:
        stMreq.imr_multiaddr.s_addr = inet_addr(hostAddr);
        stMreq.imr_interface.s_addr = INADDR_ANY;
        nRet = setsockopt(hSocket, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char *)&stMreq,
            sizeof(stMreq));

        if (nRet == SOCKET_ERROR) {
            qDebug(
                "setsockopt() IP_DROP_MEMBERSHIP address %s failed, Err: %d\n",
                hostAddr, WSAGetLastError());
        }
        _MULTIconnectOn = false;
        break;
    default:
        qDebug() << "WRONG Access to close socket";
        return -1;
    }


   // clean_buffer(_type == UDP_CLIENT? &UDPbuf : &CBuf);
    /* Close the socket */
    //closesocket(hSocket);


    /* Tell WinSock we're leaving */
   // if(!_MULTIconnectOn && !_UDPconnectOn && !_TCPconnectOn)
   //     WSACleanup();

  //  return -1;
}


