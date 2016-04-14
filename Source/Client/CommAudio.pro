#-------------------------------------------------
#
# Project created by QtCreator 2016-03-09T23:10:48
#
#-------------------------------------------------

QT       += core gui multimedia
QT       += multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CommAudio
TEMPLATE = app

LIBS += -lws2_32

SOURCES += main.cpp\
        mainwindow.cpp \
    playback.cpp \
    wavfile.cpp \
    recording.cpp \
    playlist.cpp \
    socket/circularbuffer.cpp \
    socket/clientudp.cpp \
    socket/udprecvthread.cpp \
    socket/clienttcp.cpp \
    socket/threadhandler.cpp \
    filetransfer.cpp \
    micbuffer.cpp

HEADERS  += mainwindow.h \
    playback.h \
    wavfile.h \
    recording.h \
    playlist.h \
    global.h \
    socket/circularbuffer.h \
    socket/clientudp.h \
    socket/udprecvthread.h \
    socket/clienttcp.h \
    socket/threadhandler.h \
    filetransfer.h \
    micbuffer.h

FORMS    += mainwindow.ui

RESOURCES += \
    qdarkstyle/style.qrc

CONFIG += c++11
