#-------------------------------------------------
#
# Project created by QtCreator 2014-06-18T21:03:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = alsfd
TEMPLATE = app


SOURCES += main.cpp\
        alsfd.cpp

HEADERS  += alsfd.h

LIBS += -lopencv_core -lopencv_highgui -lopencv_imgproc
unix:DEFINES = _TTY_POSIX_

PKGCONFIG    += opencv
INCLUDEPATH += /usr/include/opencv /usr/include

include(3rdparty/qextserialport/src/qextserialport.pri)

FORMS    += alsfd.ui

RESOURCES += \
    alsfd.qrc
