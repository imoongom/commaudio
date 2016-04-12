#include "threadhandler.h"
#include "clienttcp.h"
#include "clientudp.h"
#include <QDebug>

extern ClientTCP *tcpcl;
extern ClientUDP *udpCl;

ThreadHandler::ThreadHandler(QObject *parent) : QObject(parent)
{

}


DWORD WINAPI ThreadHandler::MAINThreadConnect(void * Param) {
    ThreadHandler* cthis = (ThreadHandler*)Param;
    cthis->TCPThread();
    return NULL;
}
DWORD WINAPI ThreadHandler::MAINThreadConnect2(void * Param) {
    ThreadHandler* cthis = (ThreadHandler*)Param;
    cthis->MultiThread();
    return NULL;
}

void ThreadHandler::createThread() {
    printf("### client main create Thread\n");
    HANDLE ThreadHandle, ThreadHandle2;
    HANDLE ThreadHandles[2];

    if ((ThreadHandle = CreateThread(NULL, 0, MAINThreadConnect, this, 0, NULL)) == NULL)
    {
        printf("CreateThread failed with error %d\n", GetLastError());
        return;
    }
/*
    if ((ThreadHandle2 = CreateThread(NULL, 0, MAINThreadConnect2, this, 0, NULL)) == NULL)
    {
        printf("CreateThread failed with error %d\n", GetLastError());
        return;
    }

    ThreadHandles[0] = ThreadHandle;
    ThreadHandles[1] = ThreadHandle2;
    WaitForMultipleObjects(2, ThreadHandles, TRUE, INFINITE);

*/
    WaitForSingleObject(ThreadHandle, 2000);
    printf("### TCP SERVER thread CREATED!!!!!!!!\n");
}

//1st thread function
void ThreadHandler::TCPThread() {
 //   ClientTCP tcpcl;
    if(!tcpcl->TCPConnect()){
        return ;
    }

    tcpcl->TCPcreateThread();
    qDebug()<< "TCP creat Thread";
}

//2nd thread function
void ThreadHandler::MultiThread() {
    ClientUDP cl;
    cl.Start();

}
