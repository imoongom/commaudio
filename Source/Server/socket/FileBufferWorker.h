#ifndef FILEBUFFERWORKER_H
#define FILEBUFFERWORKER_H

#include <QObject>

class FileBufferWorker : public QObject
{
    Q_OBJECT
public:
    FileBufferWorker();
private slots:
    void ReadFileAndBuffer();
signals:
    void WroteToCBuf();
};

#endif // FILEBUFFERWORKER_H
