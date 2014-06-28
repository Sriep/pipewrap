#-------------------------------------------------
#
# Project created by QtCreator 2014-06-26T18:06:58
#
#-------------------------------------------------

QT       += core
QT       -= gui

TARGET = Simbas
CONFIG   += console
CONFIG   -= app_bundle
CONFIG  += c++11

TEMPLATE = app

LIBS += -L/usr/local/hdf5/lib -lhdf5
LIBS += -L/usr/local/hdf5/lib -lhdf5_cpp
INCLUDEPATH += /usr/local/hdf5/include

SOURCES += main.cpp \
    simbax.cpp \
    baxh5.cpp

HEADERS += \
    main.h \
    simbax.h \
    baxh5.h
