#include "playback.h"

int bytesSent = 0;

Playback::Playback()
{
    qDebug() << "hi.";
}

Playback::~Playback()
{
    qDebug() << "Good bye ya";
}

void Playback::runthis()
{
    qByteArray = QByteArray();
    qBuf.setBuffer(&qByteArray);
    qBuf.open(QIODevice::ReadWrite);

    m_device = QAudioDeviceInfo::defaultOutputDevice();
    m_audioOutput = new QAudioOutput(m_device, m_device.preferredFormat());
    m_audioOutput->start(&qBuf);
}

void Playback::read_data()
{
    char *readbuf = (char*)malloc(CIRBUFSIZE);
    QByteArray qba;
    QByteArray qbaToSend;
    qint64 processedBytes;

    if (m_audioOutput->state() == 0) {
        processedBytes = (m_audioOutput->elapsedUSecs()) / m_device.preferredFormat().sampleRate() * 1000;
        //qDebug() << "processed bytes: " << processedBytes << "bytesSent: " << bytesSent;
        if (processedBytes > bytesSent) {
            bytesSent += DATA_BUFSIZE;
            qbaToSend = qba.mid(bytesSent - DATA_BUFSIZE, bytesSent);
            qDebug() << "Emitting can send at pos " << bytesSent;
            emit CanSendNextData(qbaToSend);
        }

        while(CBuf._count != 0)
        {
            read_buffer(&CBuf, readbuf);
            qba = QByteArray(readbuf, CIRBUFSIZE);
            qByteArray.append(qba);
        }
    }
}

void Playback::updateVolume(float vol)
{
    m_audioOutput->setVolume(vol);
}
