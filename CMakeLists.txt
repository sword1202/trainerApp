cmake_minimum_required(VERSION 3.12)
project(VocalTrainer)

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

set(CMAKE_CXX_STANDARD 14)

# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

set(CMAKE_AUTORCC ON)

add_compile_definitions(USE_BOOST_SERIALIZATION)

include(Qt/CMakeLists.txt)
list(TRANSFORM qtSources PREPEND Qt/)

include(Logic/CMakeLists.txt)
list(TRANSFORM logicSources PREPEND Logic/)

include(Portmixer/CMakeLists.txt)
list(TRANSFORM portMixerSources PREPEND Portmixer/)

set(cppUtilsSources
        CppUtils/TimeUtils.cpp)

set(sources ${qtSources} ${logicSources} ${portMixerSources} ${cppUtilsSources})

find_package(Qt5Widgets REQUIRED)
find_package(Qt5QuickWidgets REQUIRED)
find_package(Qt5Svg REQUIRED)

add_executable(VocalTrainer ${sources})

set_property (TARGET VocalTrainer APPEND_STRING PROPERTY
        COMPILE_FLAGS "-fobjc-arc")

target_link_libraries(VocalTrainer Qt5::Widgets)
target_link_libraries(VocalTrainer Qt5::QuickWidgets)
target_link_libraries(VocalTrainer Qt5::Svg)

if (APPLE)
    target_link_libraries(VocalTrainer "-framework Accelerate")
    target_link_libraries(VocalTrainer "-framework AppKit")
    target_link_libraries(VocalTrainer "-framework AudioToolbox")
    target_link_libraries(VocalTrainer "-framework AVFoundation")
    target_link_libraries(VocalTrainer "-framework CoreAudio")
    target_link_libraries(VocalTrainer "-framework Metal")
    target_link_libraries(VocalTrainer "-framework MetalKit")
    target_link_libraries(VocalTrainer "-framework Quartz")

    target_link_libraries(VocalTrainer ${CMAKE_SOURCE_DIR}/libs/macos/Release/libaubio.a)
    target_link_libraries(VocalTrainer ${CMAKE_SOURCE_DIR}/libs/macos/Release/libboost_serialization.a)
    target_link_libraries(VocalTrainer ${CMAKE_SOURCE_DIR}/libs/macos/Release/libportaudio.a)
    target_link_libraries(VocalTrainer ${CMAKE_SOURCE_DIR}/libs/macos/Release/libSoundTouch.1.dylib)
endif(APPLE)

if (WIN32)
    target_link_libraries(VocalTrainer User32)
    target_link_libraries(VocalTrainer Opengl32)
    target_link_libraries(VocalTrainer MFPlat)
    target_link_libraries(VocalTrainer mfuuid)
    target_link_libraries(VocalTrainer mfreadwrite)
    target_link_libraries(VocalTrainer Advapi32)
    target_link_libraries(VocalTrainer Shlwapi)
    target_link_libraries(VocalTrainer ${CMAKE_SOURCE_DIR}/libs/windows/Release/glew32s.lib)
    target_link_libraries(VocalTrainer ${CMAKE_SOURCE_DIR}/libs/windows/Release/glew32.lib)

    target_link_libraries(VocalTrainer debug ${CMAKE_SOURCE_DIR}/libs/windows/Debug/aubio.lib)
    target_link_libraries(VocalTrainer debug ${CMAKE_SOURCE_DIR}/libs/windows/Debug/libboost_serialization-vc141-mt-gd-x32-1_67.lib)
    target_link_libraries(VocalTrainer debug ${CMAKE_SOURCE_DIR}/libs/windows/Debug/portaudio.lib)
    target_link_libraries(VocalTrainer debug ${CMAKE_SOURCE_DIR}/libs/windows/Debug/SoundTouchD.lib)

    target_link_libraries(VocalTrainer general ${CMAKE_SOURCE_DIR}/libs/windows/Release/aubio.lib)
    target_link_libraries(VocalTrainer general ${CMAKE_SOURCE_DIR}/libs/windows/Release/libboost_serialization-vc141-mt-x32-1_67.lib)
    target_link_libraries(VocalTrainer general ${CMAKE_SOURCE_DIR}/libs/windows/Release/portaudio.lib)
    target_link_libraries(VocalTrainer general ${CMAKE_SOURCE_DIR}/libs/windows/Release/SoundTouch.lib)
endif(WIN32)

if (LINUX)
    target_link_libraries(VocalTrainer "GL")
    target_link_libraries(VocalTrainer "SoundTouch")
    target_link_libraries(VocalTrainer "atomic")
    target_link_libraries(VocalTrainer "pthread")
    target_link_libraries(VocalTrainer "asound")
    target_link_libraries(VocalTrainer "jack")
    target_link_libraries(VocalTrainer "avcodec")
    target_link_libraries(VocalTrainer "avformat")
    target_link_libraries(VocalTrainer "avutil")
    target_link_libraries(VocalTrainer "swresample")

    target_link_libraries(VocalTrainer debug ${CMAKE_SOURCE_DIR}/libs/linux/Debug/libaubio.a)
    target_link_libraries(VocalTrainer debug ${CMAKE_SOURCE_DIR}/libs/linux/Debug/libboost_serialization.a)
    target_link_libraries(VocalTrainer debug ${CMAKE_SOURCE_DIR}/libs/linux/Debug/libportaudio.a)
    target_link_libraries(VocalTrainer debug ${CMAKE_SOURCE_DIR}/libs/linux/Debug/libGLEW.a)

    target_link_libraries(VocalTrainer general ${CMAKE_SOURCE_DIR}/libs/linux/Release/libaubio.a)
    target_link_libraries(VocalTrainer general ${CMAKE_SOURCE_DIR}/libs/linux/Release/libboost_serialization.a)
    target_link_libraries(VocalTrainer general ${CMAKE_SOURCE_DIR}/libs/linux/Release/libportaudio.a)
    target_link_libraries(VocalTrainer general ${CMAKE_SOURCE_DIR}/libs/linux/Release/libGLEW.a)
endif(LINUX)

include_directories(
        include
        CppUtils
        Logic/AudioInput
        Logic/Drawers
        Logic/Manager
        Logic/Workspace
        Logic/Playback/Decoding
        Logic/Playback/Base
        Logic/Playback/Other
        Logic/Playback/Vx
        Logic/Playback/Mvx
        Logic/Playback/Midi/CraigsappMidifile
        Logic
        Qt
        include/nanovg
        include/nanovg/glew)
