/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: recording.cpp
--
-- PROGRAM: CommAudio
--
-- FUNCTIONS:
    explicit Recording();
    ~Recording();
    void record();
    void pause();
    void writeVoice();
    void MicDataCapturing(Recording *recording);
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
-- The class to record user's audio data from the machine's default input device.
----------------------------------------------------------------------------------------------------------------------*/
#include "recording.h"
#include "global.h"

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: Recording
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
Recording::Recording()
{
    qDebug() << "wassup";
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: ~Recording
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
Recording::~Recording()
{

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
-- Main function to run this class inside a thread. Initializes the variables and devices. Also stores data to Circular buffer.
----------------------------------------------------------------------------------------------------------------------*/
void Recording::runthis()
{
    qByteArray = QByteArray();
    qBuf.setBuffer(&qByteArray);
    qBuf.open(QIODevice::ReadWrite);
    m_format = QAudioFormat();

    m_format.setSampleRate(48000); // 48000
    m_format.setChannelCount(2);
    m_format.setSampleSize(16);
    m_format.setCodec("audio/pcm");
    m_format.setByteOrder(QAudioFormat::LittleEndian);
    m_format.setSampleType(QAudioFormat::UnSignedInt);

    m_device = QAudioDeviceInfo::defaultInputDevice();
    if (!m_device.isFormatSupported(m_format))
    {
        qWarning()<<"default format not supported try to use nearest";
        m_format = m_device.nearestFormat(m_format);
    }
    mic = new micBuffer();
    m_audioInput = new QAudioInput(m_format, mic);
    //m_audioInputDevice = m_audioInput->start(mic);
    m_audioInput->start(mic);
    emit MicDataCapturing(this);

    char *test = (char*)malloc(CIRBUFSIZE);
    return;
    while(m_audioInput->state() == QAudio::ActiveState)
    {
        qBuf.read(test, CIRBUFSIZE);
        write_buffer(&CBuf, test);
    }
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: record
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
-- DEPRECATED: Stops the recording and stores the buffer data into the circular buffer.
----------------------------------------------------------------------------------------------------------------------*/
void Recording::record()
{
    m_audioInput->resume();
    QThread::sleep(5);
    while(!qBuf.buffer().isEmpty())
    {
        qDebug() << "at leat im here";
        write_buffer(&CBuf, (qBuf.read(CIRBUFSIZE)).data());
    }
    qDebug() << "I'm done yo";
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: pause
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
-- DEPRECATED: Stops the recording and plays the audio data in the buffer.
----------------------------------------------------------------------------------------------------------------------*/
void Recording::pause()
{
    qDebug() << "woo " << qBuf.size();
    qDebug() << "hmm";
    m_audioInput->suspend();
    qBuf.seek(0);
    m_audioOutput = new QAudioOutput(QAudioDeviceInfo::defaultOutputDevice(), m_format);
    m_audioOutput->start(&qBuf);
}
