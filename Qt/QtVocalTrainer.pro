QT += quick gui core widgets quickwidgets svg multimedia

CONFIG += c++14
#QMAKE_CXXFLAGS_DEBUG += -O0

DEFINES += QT_DEPRECATED_WARNINGS

include(../PitchDetection/PitchDetection.pri)

HEADERS += \
    QmlCppBridge.h \
    qmlpitch.h \
    app.h \
    ../VocalTrainer/Playback/MvxPlayer.h \
    ../VocalTrainer/Playback/VxFile.h \
    ../VocalTrainer/Playback/MvxFile.h \
    ../VocalTrainer/Playback/VxPitch.h \
    ../VocalTrainer/Playback/VxLyricsLine.h \
    ../VocalTrainer/Playback/VxLyricsInterval.h \
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
    ../VocalTrainer/Workspace/WorkspaceDrawer.h \
    ../VocalTrainer/Workspace/PianoDrawer.h \
    ../VocalTrainer/Playback/AudioPlayback/Decoder/audiodecoder.h \
    ../VocalTrainer/Playback/AudioPlayback/Decoder/apple/CADebugMacros.h \
    ../VocalTrainer/Playback/AudioPlayback/Decoder/apple/CAStreamBasicDescription.h \
    ../VocalTrainer/Manager/AudioInputManager.h \
    ../VocalTrainer/nanovg/metal/nanovg_mtl_shaders.metal \
    ../VocalTrainer/Manager/MainController.h \
    ../VocalTrainer/Manager/AudioInputManager.h \
    ../VocalTrainer/Manager/WorkspaceZoomController.h \
    ../VocalTrainer/Drawers/QDrawer.h \
    ../VocalTrainer/Drawers/DrawerColor.h \
    ../VocalTrainer/Playback/VxFileAudioDataGenerator.h \
    ../VocalTrainer/Playback/AudioPlayback/BaseWavAudioPlayer.h \
    ../VocalTrainer/Playback/AudioPlayback/MetronomeAudioPlayer.h \
    ../VocalTrainer/Playback/AudioPlayback/AudioPlayerWithDefaultSeekHandler.h \
    ../VocalTrainer/Drawers/Drawer.h \
    ../VocalTrainer/Drawers/NvgDrawer.h \
    qmlvxpitch.h \
    qmltimedpitch.h \
    mainwindow.h \
    QtUtils/qtutils.h \
    qmlzoomcontroller.h \
    player.h \
    qopenglworkspacewidget.h \
    workspacedrawerwidgetsetup.h \
    fonts.h \
    qpainterworkspacewidget.h \
    ../VocalTrainer/Playback/AudioPlayback/Decoder/windows/qaudiodecoderwrapper.h \
    ../VocalTrainer/Playback/AudioPlayback/RealtimeStreamingAudioPlayer.h \
    ../PitchDetection/PortAudioInputReader.h \
    selectmicrophonedialog.h \
    appsettings.h \
    ../PitchDetection/AudioAverageInputLevelMonitor.h \
    welcomewindow.h \
    basemainwindow.h \
    ../VocalTrainer/Manager/PlaybackBoundsSelectionController.h

SOURCES += \
    main.cpp \
    QmlCppBridge.cpp \
    qmlpitch.cpp \
    app.cpp \
    executors.cpp \
    ../VocalTrainer/Playback/MvxPlayer.cpp \
    ../VocalTrainer/Playback/VxFile.cpp \
    ../VocalTrainer/Playback/MvxFile.cpp \
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
    ../VocalTrainer/nanovg/nanovg.cpp \
    ../VocalTrainer/Workspace/WorkspaceDrawer.cpp \
    ../VocalTrainer/Drawers/NvgDrawer.cpp \
    ../VocalTrainer/Drawers/Drawer.cpp \
    ../VocalTrainer/Playback/AudioPlayback/Decoder/audiodecoder.cpp \
    ../VocalTrainer/Manager/AudioInputManager.cpp \
    ../VocalTrainer/Manager/MainController.cpp \
    ../VocalTrainer/Manager/WorkspaceZoomController.cpp \
    ../VocalTrainer/Drawers/QDrawer.cpp \
    ../VocalTrainer/Drawers/DrawerColor.cpp \
    ../VocalTrainer/Playback/VxFileAudioDataGenerator.cpp \
    ../VocalTrainer/Playback/AudioPlayback/BaseWavAudioPlayer.cpp \
    ../VocalTrainer/Playback/AudioPlayback/MetronomeAudioPlayer.cpp \
    ../VocalTrainer/Playback/AudioPlayback/AudioPlayerWithDefaultSeekHandler.cpp \
    ../VocalTrainer/Workspace/PianoDrawer.cpp \
    ../VocalTrainer/Playback/PlaybackBounds.cpp \
    workspacedrawerwidgetsetup.cpp \
    loadtsf.cpp \
    qmlvxpitch.cpp \
    qmltimedpitch.cpp \
    mainwindow.cpp \
    QtUtils/qtutils.cpp \
    qmlzoomcontroller.cpp \
    player.cpp \
    qopenglworkspacewidget.cpp \
    fonts.cpp \
    qpainterworkspacewidget.cpp \
    ../VocalTrainer/Playback/AudioPlayback/Decoder/windows/qaudiodecoderwrapper.cpp \
    ../Portmixer/px_mac_coreaudio.c \
    ../Portmixer/px_mixer.c \
    ../VocalTrainer/Playback/AudioPlayback/RealtimeStreamingAudioPlayer.cpp \
    selectmicrophonedialog.cpp \
    appsettings.cpp \
    ../PitchDetection/AudioAverageInputLevelMonitor.cpp \
    welcomewindow.cpp \
    basemainwindow.cpp \
    ../VocalTrainer/Manager/PlaybackBoundsSelectionController.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

INCLUDEPATH += ../include \
    ../VocalTrainer/Drawers \
    ../VocalTrainer/Manager \
    ../VocalTrainer/Workspace \
    ../VocalTrainer/Playback/AudioPlayback \
    ../VocalTrainer/Playback \
    ../VocalTrainer/Playback/CraigsappMidifile \
    ../VocalTrainer \
    ../include/nanovg \
    ../include/nanovg/glew

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

macx {
    HEADERS += \
        ../VocalTrainer/Playback/AudioPlayback/Decoder/audiodecodercoreaudio_mac.h

    SOURCES += \
        appmacos.mm \
        mainwindowosx.mm \
        ../VocalTrainer/Playback/AudioPlayback/Decoder/audiodecodercoreaudio_mac.cpp


    OBJECTIVE_SOURCES = ../VocalTrainer/nanovg/metal/nanovg_mtl.m

    LIBS += -framework Foundation -framework AppKit -framework AudioToolbox -framework \
CoreFoundation -framework AVFoundation -framework CoreServices -framework CoreAudio -framework Metal -framework MetalKit -framework Quartz -framework Accelerate
    LIBS += ../libs/macos/Release/libportaudio.a
    LIBS += ../libs/macos/Release/libaubio.a
    LIBS += ../libs/macos/Release/libboost_serialization.a
    LIBS += ../libs/macos/Release/libboost_system.a
    LIBS += ../libs/macos/Release/libboost_thread.a
    LIBS += ../libs/macos/Release/libSoundTouch.1.dylib
}

win32 {
    HEADERS += \
        ../VocalTrainer/Playback/AudioPlayback/Decoder/audiodecodermediafoundation_win.h

    SOURCES += \
        ../VocalTrainer/Playback/AudioPlayback/Decoder/audiodecodermediafoundation_win.cpp

    CONFIG(debug, debug|release) {
        LIBS += ../libs/windows/Debug/SoundTouchD.lib
        LIBS += ../libs/windows/Debug/libboost_serialization-vc140-mt-gd-x32-1_67.lib
        LIBS += ../libs/windows/Debug/portaudio.lib
        LIBS += ../libs/windows/Debug/aubio.lib
    } else {
        LIBS += ../libs/windows/Release/SoundTouch.lib
        LIBS += ../libs/windows/Release/libboost_serialization-vc140-mt-x32-1_67.lib
        LIBS += ../libs/windows/Release/portaudio.lib
        LIBS += ../libs/windows/Release/aubio.lib
    }

    LIBS += ../libs/windows/Release/glew32.lib
    LIBS += ../libs/windows/Release/glew32s.lib

    LIBS += -lUser32 -lOpengl32 -lMFPlat -lmfuuid -lmfreadwrite -lAdvapi32 -lShlwapi
}

DISTFILES += \
    ../VocalTrainer/Playback/CraigsappMidifile/LICENSE.txt \
    ../VocalTrainer/Playback/a.sf2
