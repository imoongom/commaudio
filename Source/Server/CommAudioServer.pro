#-------------------------------------------------
#
# Project created by QtCreator 2016-04-11T20:42:17
#
#-------------------------------------------------

QT       += core gui multimedia
QT       += multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CommAudioServer
TEMPLATE = app

LIBS += -lws2_32

SOURCES += main.cpp\
    socket/ServerUDP.cpp \
    socket/UDPSendWorker.cpp \
    socket/TCPControlWorker.cpp \
    socket/ClientServiceWorker.cpp \
    MainWindow.cpp \
    ../Client/playback.cpp \
    ../Client/socket/circularbuffer.cpp \
    socket/FileBufferWorker.cpp

HEADERS  += \
    socket/Server.h \
    socket/ServerUDP.h \
    socket/UDPSendWorker.h \
    socket/TCPControlWorker.h \
    socket/ClientServiceWorker.h \
    MainWindow.h \
    Global.h \
    ../Client/playback.h \
    ../Client/socket/circularbuffer.h \
    socket/FileBufferWorker.h

FORMS    += mainwindow.ui

DISTFILES +=

CONFIG += c++11

RESOURCES += \
    qdarkstyle/style.qrc
