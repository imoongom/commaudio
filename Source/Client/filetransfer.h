#ifndef FILETRANSFER_H
#define FILETRANSFER_H

#include <QDebug>

#include "socket/circularbuffer.h"
#include "global.h"


class Filetransfer : public QObject
{
    Q_OBJECT

public slots:
    void sendSongName();

public:
    Filetransfer();
    ~Filetransfer();
};

#endif // FILETRANSFER_H
