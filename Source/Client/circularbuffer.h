#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

#include <cstring>
#include <stdlib.h> // malloc, free
#include "global.h"

typedef struct CircularBuffer{
    void * buffer;
    void * _writePtr;
    void * _readPtr;
    void * _endPtr;
    int _readBuf;
    int _writeBuf;
    int _count;
}CircularBufs;

//class CircularBuffer
//{

//public:
//    CircularBuffer();
    void initBuffer(CircularBuffer * Buf);
    void write_buffer(CircularBuffer * Buf, const void * data);

    void read_buffer(CircularBuffer * Buf, void * data);

    void clean_buffer(CircularBuffer * Buf);
//    CircularBuf Buf;

//private:
//    circularBuf buf;

/*
    unsigned char * buffer;
    int _endPtr;
    int _readPtr;
    int _writePtr;
*/

//};

#endif // CIRCULARBUFFER_H
