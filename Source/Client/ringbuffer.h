#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <QBuffer>
#include <QByteArray>
#include <cstring>
#include <cstdlib>

#include "global.h"

class RingBuffer
{
public:
    RingBuffer();
    ~RingBuffer();
    bool push(char * data);
    bool pop(char * data);
    bool isEmpty();
private:
    char * buffer[BUFMAX];
    int head;
    int tail;
};

#endif // RINGBUFFER_H
