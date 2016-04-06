#ifndef RECORDING_H
#define RECORDING_H

#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QAudioInput>
#include <QByteArray>
#include <QBuffer>
#include <QDebug>

#include <QAudioOutput>

class Recording
{
public:
    explicit Recording();
    ~Recording();
    void record();
    void pause();

private:
    QAudioFormat m_format;
    QAudioDeviceInfo m_device;
    QAudioInput *m_audioInput;

    QByteArray qByteArray;
    QBuffer qBuf;

    QAudioOutput *m_audioOutput;
};

#endif // RECORDING_H
