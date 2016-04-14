#include "circularbuffer.h"
#include<QDebug>
/*
CircularBuffer::CircularBuffer(){

}
*/

void initBuffer(CBufs * Buf){
    Buf->_count = 0;
    Buf->_writePtr = &Buf->buffer;
    Buf->_readPtr = &Buf->buffer;
    Buf->_endPtr = &Buf->buffer[CIRBUFSIZE*CIRBUFMAX];
    std::memset(Buf->buffer, '\0',CIRBUFSIZE*CIRBUFMAX);
    //qDebug()<<"BUFFEr SIZE: " << sizeof(Buf->buffer);
}

void write_buffer(CBufs * Buf, const void * data){

    //qDebug()<<"write buf : " << (char*)data;
    if(Buf->_count == CIRBUFMAX ){
        //qDebug() << "SIZE FULL";
        return;
    }
    if(Buf->_writePtr == Buf->_endPtr){
        qDebug() << "#######BUF END~!~~!~";
    }

    std::memcpy(Buf->_writePtr, (char*)data, CIRBUFSIZE);
    //qDebug()<<"writeptr: " << (char*)(Buf->_writePtr);
    Buf->_writePtr+=CIRBUFSIZE;
    if(Buf->_writePtr == Buf->_endPtr){
  //      qDebug() << "******END POINT";

        Buf->_writePtr = &Buf->buffer;
    }
    Buf->_count++;
    /*
    int i,cnt = 0;
    for( i = 0; &Buf->buffer[i] != Buf->_writePtr;i++){
        if(Buf->buffer[i] != '\0')
            cnt++;
    }
*/
//    qDebug() << "start Point" << &Buf->buffer<<"curPoint "<<Buf->_writePtr<< ", End Point " << Buf->_endPtr;
//    qDebug() << "COUNT: " << cnt << ", i is "<<i;
}

void read_buffer(CBufs * Buf, void * data){
    //qDebug()<<"Read buf " << Buf->_count;
    if(Buf->_count == 0)
        return ;
    std::memcpy(data, Buf->_readPtr, CIRBUFSIZE);
    std::memset(Buf->_readPtr, '\0',CIRBUFSIZE);
    Buf->_readPtr+=CIRBUFSIZE;
    if(Buf->_readPtr == Buf->_endPtr)
        Buf->_readPtr = &Buf->buffer;
    Buf->_count--;
    //qDebug()<<"read buf : " << (char*)data << " left "<< Buf->_count;

}

void clean_buffer(CBufs * Buf){
    std::memset(Buf->buffer, '\0', sizeof(Buf->buffer));
    Buf->_writePtr = Buf->buffer;
    Buf->_readPtr = Buf->buffer;
}
