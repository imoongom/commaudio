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


SOURCES += main.cpp\
        mainwindow.cpp \
    playback.cpp \
    wavfile.cpp \
    recording.cpp \
    playlist.cpp

HEADERS  += mainwindow.h \
    playback.h \
    wavfile.h \
    recording.h \
    playlist.h

FORMS    += mainwindow.ui

RESOURCES += \
    qdarkstyle/style.qrc
