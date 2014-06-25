#-------------------------------------------------
#
# Project created by QtCreator 2014-05-31T03:08:20
#
#-------------------------------------------------

QT       += core
QT       -= gui

TARGET  = VarientLister
CONFIG   += console
CONFIG   -= app_bundle
#CONFIG  += staticlib
#CONFIG += static
CONFIG  += c++11

#-i stampy_sorted.bam -t Sc8915bp_cs.fasta -o varients.csv
TEMPLATE = app

#/media/sdb2/Projects/Source/Dissertation/c++App/bamtools/lib

#DEPENDPATH += $$PWD/../bamtools/include
#INCLUDEPATH += $$PWD/../bamtools/include
#LIBS += -L$$PWD/../bamtools/lib/ -lbamtools

LIBS += -L/media/sdb2/Projects/Source/Dissertation/c++App/bamtools/lib -lbamtools
DEPENDPATH += /media/sdb2/Projects/Source/Dissertation/c++App/bamtools/include
INCLUDEPATH += /media/sdb2/Projects/Source/Dissertation/c++App/bamtools/include

LIBS += -L/usr/local/hdf5/lib -lhdf5
LIBS += -L/usr/local/hdf5/lib -lhdf5_cpp
INCLUDEPATH += /usr/local/hdf5/include

#LIBS += -L$$PWD/../hdf5/lib/ -lhdf5_cpp
#INCLUDEPATH += $$PWD/../hdf5/include

SOURCES += main.cpp \
    varientcaller.cpp \
    pvalues.cpp \
    locusinfo.cpp \
    frequencypartition.cpp \
    matchmismatches.cpp \
    hdf5basfile.cpp
HEADERS += \
    varientcaller.h \
    main.h \
    pvalues.h \
    locusinfo.h \
    frequencypartition.h \
    matchmismatches.h \
    hdf5basfile.h


