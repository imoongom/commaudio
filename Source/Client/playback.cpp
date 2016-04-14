/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: playback.cpp
--
-- PROGRAM: CommAudio
--
-- FUNCTIONS:
    Playback(struct CBuffer * buffer);
    ~Playback();
    void runthis();
    void read_data();
    void updateVolume(float vol);
    void CanSendNextData(qint64 pos, QByteArray qba);
--
-- DATE: April 14, 2016
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Eunwon Moon, Oscar Kwan, Gabriel Lee, Krystle Bulalakaw
--
-- PROGRAMMER: Eunwon Moon, Oscar Kwan, Gabriel Lee, Krystle Bulalakaw
--
-- NOTES:
-- THe class to play audio from a buffer to the machine's default output device.
----------------------------------------------------------------------------------------------------------------------*/
#include "playback.h"
#include <QAudio>

qint64 processedBytes = 0;

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: Playback
--
-- DATE: April 14, 2016
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Gabriel Lee
--
-- PROGRAMMER: Gabriel Lee
--
-- RETURNS: n/a
--
-- NOTES:
-- Constructor
----------------------------------------------------------------------------------------------------------------------*/
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

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: ~Playback
--
-- DATE: April 14, 2016
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Gabriel Lee
--
-- PROGRAMMER: Gabriel Lee
--
-- RETURNS: n/a
--
-- NOTES:
-- Destructor
----------------------------------------------------------------------------------------------------------------------*/
Playback::~Playback()
{
    qDebug() << "Good bye ya";
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: runthis
--
-- DATE: April 14, 2016
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Gabriel Lee
--
-- PROGRAMMER: Gabriel Lee
--
-- RETURNS: void
--
-- NOTES:
-- Main function to run inside a thread. Initializes all the required variables and devices.
----------------------------------------------------------------------------------------------------------------------*/
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

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: read_data
--
-- DATE: April 14, 2016
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Gabriel Lee
--
-- PROGRAMMER: Gabriel Lee
--
-- RETURNS: void
--
-- NOTES:
-- Function to read from the buffer and store into the circular buffer for networking logic.
----------------------------------------------------------------------------------------------------------------------*/
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

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: updateVolume
--
-- DATE: April 14, 2016
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Gabriel Lee
--
-- PROGRAMMER: Gabriel Lee
--
-- RETURNS: void
--
-- NOTES:
-- Updates the volume of the device.
----------------------------------------------------------------------------------------------------------------------*/
void Playback::updateVolume(float vol)
{
    m_audioOutput->setVolume(vol);
}
