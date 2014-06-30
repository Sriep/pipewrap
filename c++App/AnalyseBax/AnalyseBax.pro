#-------------------------------------------------
#
# Project created by QtCreator 2014-06-28T22:14:45
#
#-------------------------------------------------

QT       += core
QT       -= gui

TARGET = AnalyseBax

CONFIG   += console
CONFIG   -= app_bundle
CONFIG  += c++11

TEMPLATE = app

LIBS += -L/usr/local/hdf5/lib -lhdf5
LIBS += -L/usr/local/hdf5/lib -lhdf5_cpp
INCLUDEPATH += /usr/local/hdf5/include

SOURCES += main.cpp \
    hdf5basfile.cpp

HEADERS += \
    main.h \
    hdf5basfile.h
