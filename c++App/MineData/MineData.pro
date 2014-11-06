TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG  += c++11

SOURCES += main.cpp \
    options.cpp \
    generatedpsnps.cpp \
    positiondata.cpp

HEADERS += \
    options.h \
    main.h \
    generatedpsnps.h \
    positiondata.h

