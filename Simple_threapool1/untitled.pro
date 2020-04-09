TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    worker.cpp

HEADERS += \
    blockingqueue.h \
    worker.h \
    threadpool.h
