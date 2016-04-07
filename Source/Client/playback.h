#ifndef PLAYBACK_H
#define PLAYBACK_H

#include <QIODevice>
#include <QAudioDeviceInfo>
#include <QAudioOutput>
#include <QByteArray>
#include <QFile>
#include <QDebug>
#include <QtEndian>
#include <QByteArray>
#include <QBuffer>

#include "wavfile.h"
#include "circularbuffer.h"
#include "ringbuffer.h"
#include "global.h"

#include<string>
#include<iostream>

class Playback
{
public:
    explicit Playback();
    Playback(RingBuffer *buf);
    ~Playback();

    bool initialize(const QString &fileName);
    void play();
    void pause();
    void resume();

private:
    WavFile *wf;

    QAudioFormat m_format;
    QAudioDeviceInfo m_device;
    QAudioOutput *m_audioOutput;

    QByteArray qByteArray;
    QBuffer qBuf;
};

#endif // PLAYBACK_H
