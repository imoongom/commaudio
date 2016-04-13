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

public slots:
    void runthis();
    void read_data();
    void updateVolume(float vol);

signals:
    void CanSendNextData(QByteArray qba);
    void CanReadNextData(qint64 filePos);

private:
    QAudioFormat m_format;
    QAudioDeviceInfo m_device;
    QAudioOutput *m_audioOutput;

    QByteArray qByteArray;
    QBuffer qBuf;
};

#endif // PLAYBACK_H
