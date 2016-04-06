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
    int push(char * data);
    int pop(char * data);
    bool isEmpty();
private:
    char * buffer[BUFMAX];
    int head;
    int tail;
};

#endif // RINGBUFFER_H
