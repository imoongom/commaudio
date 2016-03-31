#include "recording.h"

Recording::Recording()
{
    qByteArray = QByteArray();
    qBuf.setBuffer(&qByteArray);
    qBuf.open(QIODevice::ReadWrite);
    m_format = QAudioFormat();

    m_format.setSampleRate(8000);
    m_format.setChannelCount(2);
    m_format.setSampleSize(8);
    m_format.setCodec("audio/pcm");
    m_format.setByteOrder(QAudioFormat::LittleEndian);
    m_format.setSampleType(QAudioFormat::UnSignedInt);

    m_device = QAudioDeviceInfo::defaultInputDevice();
    if (!m_device.isFormatSupported(m_format))
    {
        qWarning()<<"default format not supported try to use nearest";
        m_format = m_device.nearestFormat(m_format);
    }

    m_audioInput = new QAudioInput(m_device, m_format);
    qDebug() << "wowee";
    m_audioInput->start(&qBuf);
}

Recording::~Recording()
{

}

void Recording::record()
{

}

void Recording::pause()
{
    qDebug() << "woo " << qBuf.size();
    qDebug() << "hmm";
    m_audioInput->suspend();
    qBuf.seek(0);
    m_audioOutput = new QAudioOutput(QAudioDeviceInfo::defaultOutputDevice(), m_format);
    m_audioOutput->start(&qBuf);
}
