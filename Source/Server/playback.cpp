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
Playback::Playback()
{
    qDebug() << "hi.";
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
    qDebug() << "running this";
    qByteArray = QByteArray();
    qBuf.setBuffer(&qByteArray);
    qBuf.open(QIODevice::ReadWrite);

    m_device = QAudioDeviceInfo::defaultOutputDevice();
    m_audioOutput = new QAudioOutput(m_device, m_device.preferredFormat());
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
    char *readbuf = (char*)malloc(CIRBUFSIZE);
    QByteArray qba;
        while(CBuf._count != 0)
        {
            read_buffer(&CBuf, readbuf);
            qba = QByteArray(readbuf, CIRBUFSIZE);
            qByteArray.append(qba);
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
