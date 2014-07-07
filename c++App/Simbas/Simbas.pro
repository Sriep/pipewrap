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

#build_pass:
CONFIG(debug,debug|release) {
    DESTDIR = ../build-Pipewrap-Desktop_Qt_5_3_0_GCC_64Bit_GDB7_7_1-Debug
    message(debug)
} else {
    DESTDIR = ../build-Pipewrap-Desktop_Qt_5_3_0_GCC_64Bit_GDB7_7_1-Release
    message(release)
}
#CONFIG += debug_and_release
#CONFIG += build_all

SOURCES += main.cpp \
    simbax.cpp \
    baxh5.cpp \
    basecaller.cpp \
    constantcaller.cpp \
    bipolercaller.cpp \
    discretecaller.cpp \
    options.cpp

HEADERS += \
    main.h \
    simbax.h \
    baxh5.h \
    basecaller.h \
    constantcaller.h \
    bipolercaller.h \
    discretecaller.h \
    options.h
