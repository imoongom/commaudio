#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

#include <cstring>
#include <stdlib.h> // malloc, free
#include "global.h"

#include <cstring>
#include <stdlib.h> // malloc, free


#define CIRBUFSIZE 8192
#define CIRBUFMAX 128

typedef struct CBuffer{
    char buffer[CIRBUFSIZE*CIRBUFMAX];
    void * _writePtr;
    void * _readPtr;
    void * _endPtr;
    int _count;
}CBufs;

void initBuffer(CBufs * Buf);
void write_buffer(CBufs * Buf, const void * data);

void read_buffer(CBufs * Buf, void * data);

void clean_buffer(CBufs * Buf);


#endif // CIRCULARBUFFER_H
