QT += quick gui core widgets quickwidgets svg

CONFIG += c++14
#QMAKE_CXXFLAGS_DEBUG += -O0

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

OBJECTIVE_SOURCES = ../VocalTrainer/nanovg/metal/nanovg_mtl.m

SOURCES += \
    main.cpp \
    QmlCppBridge.cpp \
    qmlopenglitem.cpp \
    qmlpitch.cpp \
    ../PitchDetection/Pitch.cpp \
    ../PitchDetection/YinPitchDetector.cpp \
    ../PitchDetection/PitchInputReader.cpp \
    ../PitchDetection/PitchInputReaderCollector.cpp \
    ../PitchDetection/PitchesCollector.cpp \
    ../PitchDetection/PitchDetectionSmoothingAudioBuffer.cpp \
    qmlpitchinputreader.cpp \
    ../PitchDetection/CppUtils/TimeUtils.cpp \
    app.cpp \
    ../PitchDetection/PortAudio.cpp \
    ../PitchDetection/AudioInputReader.cpp \
    executors.cpp \
    ../VocalTrainer/VxPlayer/MvxPlayer.cpp \
    ../VocalTrainer/VxPlayer/VxFileAudioDataGenerator.cpp \
    ../VocalTrainer/VxPlayer/VxFile.cpp \
    ../VocalTrainer/VxPlayer/MvxFile.cpp \
    ../VocalTrainer/VxPlayer/SoundFont2PitchRenderer.cpp \
    ../VocalTrainer/VxPlayer/PitchRenderer.cpp \
    ../VocalTrainer/VxPlayer/AudioPlayback/VxFileAudioPlayer.cpp \
    ../VocalTrainer/VxPlayer/AudioPlayback/AudioPlayer.cpp \
    ../VocalTrainer/VxPlayer/AudioPlayback/AudioFilePlayer.cpp \
    ../VocalTrainer/VxPlayer/AudioPlayback/WavAudioPlayer.cpp \
    ../VocalTrainer/VxPlayer/Midi/MidiFileReader.cpp \
    ../VocalTrainer/VxPlayer/Midi/MidiTrack.cpp \
    ../VocalTrainer/VxPlayer/CraigsappMidifile/Options.cpp \
    ../VocalTrainer/VxPlayer/CraigsappMidifile/MidiMessage.cpp \
    ../VocalTrainer/VxPlayer/CraigsappMidifile/MidiFile.cpp \
    ../VocalTrainer/VxPlayer/CraigsappMidifile/MidiEventList.cpp \
    ../VocalTrainer/VxPlayer/CraigsappMidifile/MidiEvent.cpp \
    ../VocalTrainer/VxPlayer/CraigsappMidifile/Binasc.cpp \
    ../PitchDetection/WAVFile.cpp \
    ../PitchDetection/AudioUtils.cpp \
    ../PitchDetection/CppUtils/PeriodicallySleepingBackgroundTask.cpp \
    ../PitchDetection/CppUtils/SynchronizedCallbacksQueue.cpp \
    loadtsf.cpp \
    appmacos.mm \
    qmlvxpitch.cpp \
    qmltimedpitch.cpp \
    ../VocalTrainer/nanovg/nanovg.c \
    ../VocalTrainer/Workspace/WorkspaceDrawer.cpp \
    mainwindow.cpp \
    QtUtils/qtutils.cpp \
    zoomcontroller.cpp \
    ../VocalTrainer/Drawers/NvgDrawer.cpp \
    ../VocalTrainer/Drawers/Drawer.cpp \
    player.cpp \
    ../VocalTrainer/VxPlayer/AudioPlayback/Decoder/audiodecoder.cpp \
    ../VocalTrainer/VxPlayer/AudioPlayback/Decoder/audiodecodercoreaudio.cpp \
    mainwindowosx.mm \
    ../VocalTrainer/Manager/VxPitchInputReader.cpp \
    ../VocalTrainer/Manager/VxApp.cpp \
    ../PitchDetection/AubioPitchDetector.cpp \
    MacOS/qmetalwidget.mm \
    MacOS/workspaceview.mm

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

macx {
    LIBS += -framework Foundation -framework AppKit -framework AudioToolbox -framework \
CoreFoundation -framework AVFoundation -framework CoreServices -framework CoreAudio -framework Metal -framework MetalKit -framework Quartz -framework Accelerate
    LIBS += ../libs/Release/libportaudio.a
    LIBS += ../libs/Release/libaubio.a
    LIBS += ../libs/Release/libboost_serialization.a
    LIBS += ../libs/Release/libSoundTouch.1.dylib
}

HEADERS += \
    QmlCppBridge.h \
    qmlopenglitem.h \
    qmlpitch.h \
    ../PitchDetection/Pitch.h \
    ../PitchDetection/PitchDetectionSmoothingAudioBuffer.h \
    ../PitchDetection/YinPitchDetector.h \
    ../PitchDetection/PitchInputReader.h \
    ../PitchDetection/PitchInputReaderCollector.h \
    ../PitchDetection/AudioInputReader.h \
    qmlpitchinputreader.h \
    ../PitchDetection/CppUtils/TimeUtils.h \
    app.h \
    ../PitchDetection/PortAudio.h \
    ../VocalTrainer/VxPlayer/MvxPlayer.h \
    ../VocalTrainer/VxPlayer/VxFile.h \
    ../VocalTrainer/VxPlayer/VxFileAudioDataGenerator.h \
    ../VocalTrainer/VxPlayer/MvxFile.h \
    ../VocalTrainer/VxPlayer/VxPitch.h \
    ../VocalTrainer/VxPlayer/VxLyricsLine.h \
    ../VocalTrainer/VxPlayer/VxLyricsInterval.h \
    ../VocalTrainer/VxPlayer/SoundFont2PitchRenderer.h \
    ../VocalTrainer/VxPlayer/PitchRenderer.h \
    ../VocalTrainer/VxPlayer/LoadTsf.h \
    ../VocalTrainer/VxPlayer/tsf.h \
    ../VocalTrainer/VxPlayer/AudioPlayback/VxFileAudioPlayer.h \
    ../VocalTrainer/VxPlayer/AudioPlayback/AudioPlayer.h \
    ../VocalTrainer/VxPlayer/AudioPlayback/AudioFilePlayer.h \
    ../VocalTrainer/VxPlayer/AudioPlayback/WavAudioPlayer.h \
    ../VocalTrainer/VxPlayer/Midi/MidiFileReader.h \
    ../VocalTrainer/VxPlayer/Midi/MidiNote.h \
    ../VocalTrainer/VxPlayer/Midi/MidiTrack.h \
    ../VocalTrainer/VxPlayer/CraigsappMidifile/MidiFile.h \
    ../VocalTrainer/VxPlayer/CraigsappMidifile/Options.h \
    ../VocalTrainer/VxPlayer/CraigsappMidifile/MidiMessage.h \
    ../VocalTrainer/VxPlayer/CraigsappMidifile/MidiEventList.h \
    ../VocalTrainer/VxPlayer/CraigsappMidifile/MidiEvent.h \
    ../VocalTrainer/VxPlayer/CraigsappMidifile/Binasc.h \
    ../PitchDetection/WAVFile.h \
    ../PitchDetection/AudioUtils.h \
    ../PitchDetection/CppUtils/PeriodicallySleepingBackgroundTask.h \
    ../PitchDetection/CppUtils/SynchronizedCallbacksQueue.h \
    qmlvxpitch.h \
    qmltimedpitch.h \
    ../VocalTrainer/Workspace/WorkspaceDrawer.h \
    mainwindow.h \
    QtUtils/qtutils.h \
    zoomcontroller.h \
    ../VocalTrainer/Drawers/Drawer.h \
    ../VocalTrainer/Drawers/NvgDrawer.h \
    player.h \
    ../VocalTrainer/VxPlayer/AudioPlayback/Decoder/audiodecoder.h \
    ../VocalTrainer/VxPlayer/AudioPlayback/Decoder/audiodecodercoreaudio.h \
    ../VocalTrainer/VxPlayer/AudioPlayback/Decoder/apple/CADebugMacros.h \
    ../VocalTrainer/VxPlayer/AudioPlayback/Decoder/apple/CAStreamBasicDescription.h \
    ../VocalTrainer/Manager/VxPitchInputReader.h \
    ../PitchDetection/AubioPitchDetector.h \
    ../VocalTrainer/nanovg/metal/nanovg_mtl_shaders.metal \
    MacOS/qmetalwidget.h \
    MacOS/workspaceview.h

INCLUDEPATH += ../include \
    ../VocalTrainer/Drawers \
    ../VocalTrainer/Manager \
    ../VocalTrainer/Workspace \
    ../VocalTrainer/VxPlayer/AudioPlayback \
    ../VocalTrainer/VxPlayer \
    ../VocalTrainer/VxPlayer/CraigsappMidifile \
    ../VocalTrainer/ \
    ../PitchDetection/CppUtils \
    ../PitchDetection \
    ../include/nanovg

DISTFILES += \
    ../VocalTrainer/VxPlayer/CraigsappMidifile/LICENSE.txt \
    ../VocalTrainer/VxPlayer/a.sf2

