CONFIG -= qt 

TARGET = CppUtils
TEMPLATE = lib

HEADERS += \
    PeriodicallySleepingBackgroundTask.h \
    SynchronizedCallbacksQueue.h \
    TimeUtils.h

SOURCES += \
    PeriodicallySleepingBackgroundTask.cpp \
    SynchronizedCallbacksQueue.cpp \
    TimeUtils.cpp


INCLUDEPATH += ../../include
