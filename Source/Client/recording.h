#ifndef RECORDING_H
#define RECORDING_H

#include <QObject>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QAudioInput>
#include <QByteArray>
#include <QBuffer>
#include <QDebug>

#include <QAudioOutput>

#include<QThread>

#include "socket/circularbuffer.h"

class Recording : public QObject
{
    Q_OBJECT

public:
    explicit Recording();
    ~Recording();
    void record();
    void pause();

public slots:
    void runthis();

private:
    QAudioFormat m_format;
    QAudioDeviceInfo m_device;
    QAudioInput *m_audioInput;

    QByteArray qByteArray;
    QBuffer qBuf;

    QAudioOutput *m_audioOutput;
};

#endif // RECORDING_H
