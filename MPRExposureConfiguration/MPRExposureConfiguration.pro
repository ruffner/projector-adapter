QT += core gui widgets
TEMPLATE = app
TARGET = MPRExposureConfiguration


SOURCES += main.cpp\
           mprexposuredialog.cpp


HEADERS += mprexposuredialog.h

LIBS += -L/usr/local/lib -lusb-1.0
