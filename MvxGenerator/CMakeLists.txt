cmake_minimum_required(VERSION 3.12)
project(MvxGenerator)

set(CMAKE_CXX_STANDARD 14)

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

include(../Logic/Playback/playback.cmake)
include(../cpputils.cmake)

set(sources ${playbackSources}
        ${cppUtilsSources}
        ../CppUtils/Executors.mm
        ../Logic/AudioInput/Pitch.cpp
        ../Qt/Utils/QtUtils.cpp
        Handler.h
        ../Qt/App/LoadTsf.cpp)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5QuickWidgets REQUIRED)
find_package(Qt5Svg REQUIRED)

add_executable(MvxGenerator ${sources} main.cpp main.qrc)

set_property (TARGET MvxGenerator APPEND_STRING PROPERTY
        COMPILE_FLAGS "-fobjc-arc")

target_link_libraries(MvxGenerator Qt5::Widgets)
target_link_libraries(MvxGenerator Qt5::QuickWidgets)
target_link_libraries(MvxGenerator Qt5::Svg)

include(../include.cmake)

if (APPLE)
    target_link_libraries(MvxGenerator "-framework Accelerate")
    target_link_libraries(MvxGenerator "-framework AppKit")
    target_link_libraries(MvxGenerator "-framework AudioToolbox")
    target_link_libraries(MvxGenerator "-framework AVFoundation")
    target_link_libraries(MvxGenerator "-framework CoreAudio")

    target_link_libraries(MvxGenerator ${CMAKE_SOURCE_DIR}/../libs/macos/Release/libboost_serialization.a)
    target_link_libraries(MvxGenerator ${CMAKE_SOURCE_DIR}/../libs/macos/Release/libportaudio.a)
    target_link_libraries(MvxGenerator ${CMAKE_SOURCE_DIR}/../libs/macos/Release/libSoundTouch.1.dylib)
endif(APPLE)