#ifndef RECORDING_H
#define RECORDING_H

#include <QObject>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QAudioInput>
#include <QByteArray>
#include <QBuffer>
#include <QDebug>
#include "micbuffer.h"
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
    QIODevice *m_audioInputDevice;
    QIODevice *m_audioOutputDevice;
    QAudioFormat m_format;
    QAudioDeviceInfo m_device;
    QAudioInput *m_audioInput;
    micBuffer * mic;
    QByteArray qByteArray;
    QBuffer qBuf;

    QAudioOutput *m_audioOutput;

signals:
    void writeVoice();
    void MicDataCapturing(Recording *recording);

public slots:
    void runthis();

private:
};

#endif // RECORDING_H
