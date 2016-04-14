#ifndef MICBUFFER_H
#define MICBUFFER_H
#include <QIODevice>

class micBuffer : public QIODevice
{
public:
    micBuffer();
    virtual qint64 readData(char * data, qint64 maxSize);
    virtual qint64 writeData(const char * data, qint64 maxSize);
    qint64 bytesAvailable();
    char * buffer;
    char buf[8192];
    int pos = 0;
};

#endif // MICBUFFER_H
