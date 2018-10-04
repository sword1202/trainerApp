cmake_minimum_required(VERSION 3.12)
project(VocalTrainer)

set(CMAKE_CXX_STANDARD 14)

# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

set(CMAKE_AUTORCC ON)

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

target_link_libraries(VocalTrainer Qt5::Widgets)
target_link_libraries(VocalTrainer Qt5::QuickWidgets)
target_link_libraries(VocalTrainer Qt5::Svg)

if (APPLE)
    target_link_libraries(VocalTrainer "-framework Accelerate")
    target_link_libraries(VocalTrainer "-framework AppKit")
    target_link_libraries(VocalTrainer "-framework AudioToolbox")
    target_link_libraries(VocalTrainer "-framework AVFoundation")
    target_link_libraries(VocalTrainer "-framework CoreAudio")
    target_link_libraries(VocalTrainer "-framework CoreFoundation")
    target_link_libraries(VocalTrainer "-framework CoreServices")
    target_link_libraries(VocalTrainer "-framework Foundation")
    target_link_libraries(VocalTrainer "-framework Metal")
    target_link_libraries(VocalTrainer "-framework Quartz")
    target_link_libraries(VocalTrainer "-framework OpenGL")

    target_link_libraries(VocalTrainer ${CMAKE_SOURCE_DIR}/libs/macos/Release/libaubio.a)
    target_link_libraries(VocalTrainer ${CMAKE_SOURCE_DIR}/libs/macos/Release/libboost_serialization.a)
    target_link_libraries(VocalTrainer ${CMAKE_SOURCE_DIR}/libs/macos/Release/libportaudio.a)
    target_link_libraries(VocalTrainer ${CMAKE_SOURCE_DIR}/libs/macos/Release/libSoundTouch.1.dylib)
endif(APPLE)

include_directories(
        include
        CppUtils
        Logic/PitchDetection
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
        include/nanovg
        include/nanovg/glew)