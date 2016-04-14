#include "playback.h"
#include <QAudio>

qint64 processedBytes = 0;

Playback::Playback(struct CBuffer * buffer)
{
    qDebug() << "hi.";
    playBuf = buffer;
    m_format = QAudioFormat();

    m_format.setSampleRate(44100); // 48000
    m_format.setChannelCount(2);
    m_format.setSampleSize(16);
    m_format.setCodec("audio/pcm");
    m_format.setByteOrder(QAudioFormat::LittleEndian);
    m_format.setSampleType(QAudioFormat::UnSignedInt);


}

Playback::~Playback()
{
    qDebug() << "Good bye ya";
}

void Playback::runthis()
{

    if ((fstream = fopen("NeYo-SoSick.wav", "awb+")) == NULL) {
        qDebug()<<"FILE OPEN FAIL";
        return;
    }

    qDebug()<<"runthis";
    qByteArray = QByteArray();
    qBuf.setBuffer(&qByteArray);
    qBuf.open(QIODevice::ReadWrite);

    m_device = QAudioDeviceInfo::defaultOutputDevice();
    m_audioOutput = new QAudioOutput(m_device, m_format);
    m_audioOutput->start(&qBuf);
}
void Playback::read_data()
{
    qDebug()<<"READ DATA" << playBuf->_count;
    char *readbuf = (char*)malloc(CIRBUFSIZE);
    QByteArray qba;

    // While audio is playing

         while(playBuf->_count != 0)
        {
            read_buffer(playBuf, readbuf);
            qba = QByteArray(readbuf, CIRBUFSIZE);
            qByteArray.append(qba);
            fwrite(readbuf, 1, CIRBUFSIZE, fstream);
            if(m_audioOutput->state() == QAudio::SuspendedState)
                m_audioOutput->resume();
            if(m_audioOutput->state() == QAudio::IdleState){
                qDebug() << "Resume player";
                m_audioOutput->resume();
                m_audioOutput->start(&qBuf);
            }
            if(m_audioOutput->state() == QAudio::StoppedState){
                m_audioOutput->start(&qBuf);
            }
            if(m_audioOutput->state() == QAudio::ActiveState){
                qDebug() << "Player active";
            }

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
