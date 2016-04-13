#include "playback.h"


qint64 processedBytes = 0;

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

void Playback::read_data(qint64 pos)
{
    char *readbuf = (char*)malloc(CIRBUFSIZE);
    QByteArray qba;
    QByteArray qbaToSend;

    // While audio is playing
    if (m_audioOutput->state() == 0) {
        processedBytes = (m_audioOutput->elapsedUSecs() / m_device.preferredFormat().sampleRate());
        qDebug() << "processed bytes: " << processedBytes << "bytesSent: " << pos;
        while(playBuf->_count != 0)
        {
            read_buffer(playBuf, readbuf);
            qba = QByteArray(readbuf, CIRBUFSIZE);
            qByteArray.append(qba);
            qbaToSend = qba.mid(pos, pos + DATA_BUFSIZE);
            // should emit when the playback has reached end of bufsize
            emit CanSendNextData(pos + DATA_BUFSIZE, qbaToSend);
            /*
            if (processedBytes >= pos) {
                qDebug() << "Emitting can send at pos " << pos;
                pos += DATA_BUFSIZE;
                emit CanSendNextData(pos, qbaToSend);
            }
            */
        }
    }
}

void Playback::updateVolume(float vol)
{
    m_audioOutput->setVolume(vol);
}
