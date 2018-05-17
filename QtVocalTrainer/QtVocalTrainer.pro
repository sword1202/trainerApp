QT += quick

QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS_DEBUG += -O0

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    MainWindow.mm \
    main.cpp \
    QmlCppBridge.cpp \
    MainWindowShared.cpp \
    qmlopenglitem.cpp \
    qmlpitch.cpp \
    ../PitchDetection/Pitch.cpp \
    ../PitchDetection/AudoInputReader.cpp \
    ../PitchDetection/YinPitchDetector.cpp \
    ../PitchDetection/PitchInputReader.cpp \
    ../PitchDetection/PitchDetectionSmoothingAudioBuffer.cpp \
    qmlpitchinputreader.cpp \
    ../PitchDetection/CppUtils/TimeUtils.cpp \
    app.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

LIBS += -framework Foundation -framework AppKit -framework AudioToolbox -framework CoreFoundation

HEADERS += \
    MainWindow.h \
    QmlCppBridge.h \
    qmlopenglitem.h \
    qmlpitch.h \
    ../PitchDetection/Pitch.h \
    ../PitchDetection/PitchDetectionSmoothingAudioBuffer.h \
    ../PitchDetection/YinPitchDetector.h \
    ../PitchDetection/PitchInputReader.h \
    ../PitchDetection/AudoInputReader.h \
    qmlpitchinputreader.h \
    ../PitchDetection/CppUtils/TimeUtils.h \
    app.h

INCLUDEPATH += ../include

DISTFILES +=
