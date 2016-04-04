#include "circularbuffer.h"

/*
CircularBuffer::CircularBuffer()

}
*/
void initBuffer(CircularBuffer * Buf){
    Buf->buffer = malloc(BUFSIZE * BUFMAX);
    Buf->_count = 0;
    Buf->_readBuf = 0;
    Buf->_writeBuf = 0;
    Buf->_writePtr = Buf->buffer;
    Buf->_readPtr = Buf->buffer;
    Buf->_endPtr = Buf->buffer + BUFSIZE*BUFMAX;
    //Buf->_endPtr;// = buffer + sizeof(buffer);
}

void write_buffer(CircularBuffer * Buf, const void * data){
    if(Buf->_count == BUFSIZE)
        return;

    std::memcpy(Buf->_writePtr, data, BUFSIZE);
    Buf->_writeBuf++;
    if(Buf->_writePtr == Buf->_endPtr)
        Buf->_writePtr = Buf->buffer;
    Buf->_count++;
}

void read_buffer(CircularBuffer * Buf, void * data){
    if(Buf->_count == 0)
        return ;
    std::memcpy(data, Buf->_readPtr, BUFSIZE);
    Buf->_readBuf++;
    if(Buf->_readPtr == Buf->_endPtr)
        Buf->_readPtr = Buf->buffer;
    Buf->_count--;

}

void clean_buffer(CircularBuffer * Buf){
    std::memset(Buf->buffer, 0, sizeof(Buf->buffer));
    Buf->_readBuf = 0;
    Buf->_writeBuf = 0;
    Buf->_writePtr = Buf->buffer;
    Buf->_readPtr = Buf->buffer;
}
