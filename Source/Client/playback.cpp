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
    qDebug()<<"runthis";
    qByteArray = QByteArray();
    qBuf.setBuffer(&qByteArray);
    qBuf.open(QIODevice::ReadWrite);

    m_device = QAudioDeviceInfo::defaultOutputDevice();
    m_audioOutput = new QAudioOutput(m_device, m_device.preferredFormat());
    m_audioOutput->start(&qBuf);
}
void Playback::read_data()
{
    qDebug()<<"READ DATA" << CBuf._count;
    char *readbuf = (char*)malloc(CIRBUFSIZE);
    QByteArray qba;

    // While audio is playing

         while(CBuf._count != 0)
        {
            read_buffer(&CBuf, readbuf);
            qba = QByteArray(readbuf, CIRBUFSIZE);
            qByteArray.append(qba);
        }

}

/* eunwon's, previous version w/o parameter above
void Playback::read_data()
{
    qDebug()<<"READ DATA" << playBuf->_count;
    char *readbuf = (char*)malloc(CIRBUFSIZE);
    QByteArray qba;
    QByteArray qbaToSend;

    // While audio is playing
    if (m_audioOutput->state() == 0) {
        char *readbuf = (char*)malloc(CIRBUFSIZE);
        QByteArray qba;
        while(playBuf->_count != 0)
        {
            read_buffer(playBuf, readbuf);
            qba = QByteArray(readbuf, CIRBUFSIZE);
            qByteArray.append(qba);
            qbaToSend = qba.mid(pos, pos + DATA_BUFSIZE);
        }
    }
}
*/

void Playback::updateVolume(float vol)
{
    m_audioOutput->setVolume(vol);
}
