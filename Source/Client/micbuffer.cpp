#include "micbuffer.h"
#include <QDebug>
#include "socket/circularbuffer.h"
#include "global.h"
//extern struct CBuffer UDPbuf;

micBuffer::micBuffer()
{
    this->open(QIODevice::WriteOnly);
    qDebug() << "What does eunwon know???";
    pos = 0;
    buffer = buf;
}
qint64 micBuffer::readData(char * data, qint64 maxSize){
    return 0;
}

qint64 micBuffer::writeData(const char * data, qint64 maxSize){
    if((pos + maxSize) < 8192){
        buffer += pos;
        memcpy(buffer, data, maxSize);
        printf("Adding : %d", maxSize);
        pos += maxSize;

    }else{
        int rem = 8192 - pos;
        buffer += pos;
        memcpy(buffer, data, rem);
        printf("adding not full :%d", rem);
        pos = 0;
        buffer = &buf[0];
        write_buffer(&UDPbuf, buffer);
        return rem;
    }

    return maxSize;
}

qint64 micBuffer::bytesAvailable(){

}
