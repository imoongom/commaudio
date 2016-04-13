#include "playback.h"


Playback::Playback(struct CBuffer * buffer)
{
    qDebug() << "hi.";
    playBuf = buffer;
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
        while(playBuf->_count != 0)
        {
            read_buffer(playBuf, readbuf);
            qba = QByteArray(readbuf, CIRBUFSIZE);
            qByteArray.append(qba);
        }
}

void Playback::updateVolume(float vol)
{
    m_audioOutput->setVolume(vol);
}
