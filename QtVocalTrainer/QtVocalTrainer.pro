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
    ../VocalTrainer/Playback/MvxPlayer.cpp \
    ../VocalTrainer/Playback/ParallelVxFileAudioDataGenerator.cpp \
    ../VocalTrainer/Playback/VxFile.cpp \
    ../VocalTrainer/Playback/MvxFile.cpp \
    ../VocalTrainer/Playback/SoundFont2PitchRenderer.cpp \
    ../VocalTrainer/Playback/PitchRenderer.cpp \
    ../VocalTrainer/Playback/AudioPlayback/VxFileAudioPlayer.cpp \
    ../VocalTrainer/Playback/AudioPlayback/AudioPlayer.cpp \
    ../VocalTrainer/Playback/AudioPlayback/AudioFilePlayer.cpp \
    ../VocalTrainer/Playback/AudioPlayback/WavAudioPlayer.cpp \
    ../VocalTrainer/Playback/Midi/MidiFileReader.cpp \
    ../VocalTrainer/Playback/Midi/MidiTrack.cpp \
    ../VocalTrainer/Playback/CraigsappMidifile/Options.cpp \
    ../VocalTrainer/Playback/CraigsappMidifile/MidiMessage.cpp \
    ../VocalTrainer/Playback/CraigsappMidifile/MidiFile.cpp \
    ../VocalTrainer/Playback/CraigsappMidifile/MidiEventList.cpp \
    ../VocalTrainer/Playback/CraigsappMidifile/MidiEvent.cpp \
    ../VocalTrainer/Playback/CraigsappMidifile/Binasc.cpp \
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
    qmlzoomcontroller.cpp \
    ../VocalTrainer/Drawers/NvgDrawer.cpp \
    ../VocalTrainer/Drawers/Drawer.cpp \
    player.cpp \
    ../VocalTrainer/Playback/AudioPlayback/Decoder/audiodecoder.cpp \
    ../VocalTrainer/Playback/AudioPlayback/Decoder/audiodecodercoreaudio.cpp \
    mainwindowosx.mm \
    ../VocalTrainer/Manager/VxPitchInputReader.cpp \
    ../VocalTrainer/Manager/MainController.cpp \
    ../PitchDetection/AubioPitchDetector.cpp \
    ../VocalTrainer/Manager/ZoomController.cpp \
    ../VocalTrainer/Drawers/QDrawer.cpp \
    ../VocalTrainer/Drawers/DrawerColor.cpp \
    qopenglworkspacewidget.cpp \
    ../VocalTrainer/Playback/VxFileAudioDataGenerator.cpp

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
    ../VocalTrainer/Playback/MvxPlayer.h \
    ../VocalTrainer/Playback/VxFile.h \
    ../VocalTrainer/Playback/ParallelVxFileAudioDataGenerator.h \
    ../VocalTrainer/Playback/MvxFile.h \
    ../VocalTrainer/Playback/VxPitch.h \
    ../VocalTrainer/Playback/VxLyricsLine.h \
    ../VocalTrainer/Playback/VxLyricsInterval.h \
    ../VocalTrainer/Playback/SoundFont2PitchRenderer.h \
    ../VocalTrainer/Playback/PitchRenderer.h \
    ../VocalTrainer/Playback/LoadTsf.h \
    ../VocalTrainer/Playback/tsf.h \
    ../VocalTrainer/Playback/AudioPlayback/VxFileAudioPlayer.h \
    ../VocalTrainer/Playback/AudioPlayback/AudioPlayer.h \
    ../VocalTrainer/Playback/AudioPlayback/AudioFilePlayer.h \
    ../VocalTrainer/Playback/AudioPlayback/WavAudioPlayer.h \
    ../VocalTrainer/Playback/Midi/MidiFileReader.h \
    ../VocalTrainer/Playback/Midi/MidiNote.h \
    ../VocalTrainer/Playback/Midi/MidiTrack.h \
    ../VocalTrainer/Playback/CraigsappMidifile/MidiFile.h \
    ../VocalTrainer/Playback/CraigsappMidifile/Options.h \
    ../VocalTrainer/Playback/CraigsappMidifile/MidiMessage.h \
    ../VocalTrainer/Playback/CraigsappMidifile/MidiEventList.h \
    ../VocalTrainer/Playback/CraigsappMidifile/MidiEvent.h \
    ../VocalTrainer/Playback/CraigsappMidifile/Binasc.h \
    ../PitchDetection/WAVFile.h \
    ../PitchDetection/AudioUtils.h \
    ../PitchDetection/CppUtils/PeriodicallySleepingBackgroundTask.h \
    ../PitchDetection/CppUtils/SynchronizedCallbacksQueue.h \
    qmlvxpitch.h \
    qmltimedpitch.h \
    ../VocalTrainer/Workspace/WorkspaceDrawer.h \
    mainwindow.h \
    QtUtils/qtutils.h \
    qmlzoomcontroller.h \
    ../VocalTrainer/Drawers/Drawer.h \
    ../VocalTrainer/Drawers/NvgDrawer.h \
    player.h \
    ../VocalTrainer/Playback/AudioPlayback/Decoder/audiodecoder.h \
    ../VocalTrainer/Playback/AudioPlayback/Decoder/audiodecodercoreaudio.h \
    ../VocalTrainer/Playback/AudioPlayback/Decoder/apple/CADebugMacros.h \
    ../VocalTrainer/Playback/AudioPlayback/Decoder/apple/CAStreamBasicDescription.h \
    ../VocalTrainer/Manager/VxPitchInputReader.h \
    ../PitchDetection/AubioPitchDetector.h \
    ../VocalTrainer/nanovg/metal/nanovg_mtl_shaders.metal \
    ../VocalTrainer/Manager/MainController.h \
    ../VocalTrainer/Manager/VxPitchInputReader.h \
    ../VocalTrainer/Manager/ZoomController.h \
    ../VocalTrainer/Drawers/QDrawer.h \
    ../VocalTrainer/Drawers/DrawerColor.h \
    qopenglworkspacewidget.h \
    ../VocalTrainer/Playback/VxFileAudioDataGenerator.h

INCLUDEPATH += ../include \
    ../VocalTrainer/Drawers \
    ../VocalTrainer/Manager \
    ../VocalTrainer/Workspace \
    ../VocalTrainer/Playback/AudioPlayback \
    ../VocalTrainer/Playback \
    ../VocalTrainer/Playback/CraigsappMidifile \
    ../VocalTrainer/ \
    ../PitchDetection/CppUtils \
    ../PitchDetection \
    ../include/nanovg

DISTFILES += \
    ../VocalTrainer/Playback/CraigsappMidifile/LICENSE.txt \
    ../VocalTrainer/Playback/a.sf2

