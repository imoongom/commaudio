#include "clientudp.h"
#include "../global.h"

#include <QDebug>

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	Start
--
-- DATE: 		March 21, 2016
--
-- REVISIONS:
--
-- DESIGNER	 : 	Eunwon Moon
--
-- PROGRAMMER: 	Eunwon Moon
--
-- INTERFACE: 	boolean Start(SOCKET* sock, int port)
--              SOCKET : socket pointer from mainwindow
--              port   : port number
--
-- RETURNS: boolean - the connection success or fail
--
-- NOTES:
--	This function is to open udp client or multicast client.
--  open socket, setting socket option, and bind
----------------------------------------------------------------------------------------------------------------------*/
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

    fFlag = TRUE;
    nRet = setsockopt(hSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&fFlag,
                        sizeof(fFlag));
    if (nRet == SOCKET_ERROR) {
        qDebug("setsockopt() SO_REUSEADDR failed, Err: %d\n",
            WSAGetLastError());
        return false;
    }

    /* setup local socket*/
    locAddr.sin_family = AF_INET;
    locAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    locAddr.sin_port = htons(nPort);


    nRet = bind(hSocket, (struct sockaddr*) &locAddr, sizeof(locAddr));
    if (nRet == SOCKET_ERROR) {
        qDebug("bind() port: %d failed, Err: %d\n", nPort,
            WSAGetLastError());
        return false;
    }

    return true;
}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	multiSetup
--
-- DATE: 		March 21, 2016
--
-- REVISIONS:
--
-- DESIGNER	 : 	Eunwon Moon
--
-- PROGRAMMER: 	Eunwon Moon
--
-- INTERFACE: 	boolean multiSetup(SOCKET* sock)
--              SOCKET : socket pointer from mainwindow - multicast socket
--
-- RETURNS: boolean - the connection success or fail
--
-- NOTES:
--	This function is to change socket option for multisetup.
--  join the multi cast group
----------------------------------------------------------------------------------------------------------------------*/
boolean ClientUDP::multiSetup(SOCKET *sock){
    int nRet;

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
    return true;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	udpConn
--
-- DATE: 		April 3, 2016
--
-- REVISIONS:
--
-- DESIGNER	 : 	Eunwon Moon
--
-- PROGRAMMER: 	Eunwon Moon
--
-- INTERFACE: 	void udpConn
--
-- RETURNS: void
--
-- NOTES:
--	This function is to connect to udp server for voice chat.
--  it directly connect to udp for set voice message.
----------------------------------------------------------------------------------------------------------------------*/
void ClientUDP::udpConn(){

    DWORD nRet;
    WSADATA stWSAData;
    char ip[15] = UDP_DEFAULT_PORT;
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

    SIVoice->Socket = voiceTo;
    SIVoice->DataBuf.len = DATA_BUFSIZE;

    memset(&InetAddr, 0, sizeof(InetAddr));
    InetAddr.sin_family = AF_INET;
    InetAddr.sin_port = htons(UDP_DEFAULT_PORT);

   // memcpy((char *)&InetAddr.sin_addr.s_addr, voiceHost->h_addr, voiceHost->h_length);
    if((InetAddr.sin_addr.s_addr = inet_addr(ip))==INADDR_NONE)
    {
        qDebug() << "INVALID IP ";
    }

    qDebug()<<"[VoiceSend] SETUP FINISHED " << _VoiceChat << " " << voiceTo;
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	sendVoice
--
-- DATE: 		April 10, 2016
--
-- REVISIONS:
--
-- DESIGNER	 : 	Eunwon Moon
--
-- PROGRAMMER: 	Eunwon Moon
--
-- INTERFACE: 	void udpConn
--
-- RETURNS: void
--
-- NOTES:
--	This function is to send voice to use UDP connection
----------------------------------------------------------------------------------------------------------------------*/
void ClientUDP::sendVoice(){//char *ip){
    char temp[DATA_BUFSIZE];
    DWORD  SendBytes;

    if (_VoiceChat) {
          qDebug() << "Voice Chat startL " <<UDPbuf._count;
       SIVoice->DataBuf.buf = temp;
        if(UDPbuf._count == 0)
            return;

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
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	UDPClose
--
-- DATE: 		April 5, 2016
--
-- REVISIONS:
--
-- DESIGNER	 : 	Eunwon Moon
--
-- PROGRAMMER: 	Eunwon Moon
--
-- INTERFACE: 	void udpConn
--
-- RETURNS: void
--
-- NOTES:
--	This function is to send voice to use UDP connection
----------------------------------------------------------------------------------------------------------------------*/
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
    closesocket(hSocket);


    /* Tell WinSock we're leaving */
    if(!_MULTIconnectOn && !_UDPconnectOn && !_TCPconnectOn)
        WSACleanup();

  //  return -1;
}


