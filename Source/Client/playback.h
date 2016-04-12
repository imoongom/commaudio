#ifndef PLAYBACK_H
#define PLAYBACK_H

#include <QObject>
#include <QIODevice>
#include <QAudioDeviceInfo>
#include <QAudioOutput>
#include <QByteArray>
#include <QFile>
#include <QDebug>
#include <QtEndian>
#include <QByteArray>
#include <QBuffer>
#include <QtConcurrent/QtConcurrent>
#include <QDataStream>

#include "wavfile.h"
#include "socket/circularbuffer.h"
#include "global.h"

#include<string>
#include<iostream>

class Playback : public QObject
{
    Q_OBJECT

public:
    Playback();
    ~Playback();

    void write_data();

public slots:
    void runthis();
    void read_data();

private:
    WavFile *wf;

    QAudioFormat m_format;
    QAudioDeviceInfo m_device;
    QAudioOutput *m_audioOutput;

    QByteArray qByteArray;
    QBuffer qBuf;

    CBufs *bufs;

    int running = 1;
};

#endif // PLAYBACK_H
