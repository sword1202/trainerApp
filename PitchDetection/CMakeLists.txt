cmake_minimum_required(VERSION 3.5)
project(PitchDetection)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -framework AudioToolbox -framework CoreFoundation -framework Accelerate")

set(SOURCE_FILES
        AudioInputReader.cpp
        AudioInputReader.h
        PitchInputReader.cpp
        PitchInputReader.h
        Pitch.cpp
        Pitch.h
        CppUtils/Algorithms.h
        CppUtils/stlassert.h
        PitchDetectionSmoothingAudioBuffer.h
        PitchDetectionSmoothingAudioBuffer.cpp
        AudioUtils.h
        AudioUtils.cpp
        CppUtils/TimeUtils.h
        CppUtils/TimeUtils.cpp
        WAVFile.cpp
        WAVFile.h
        WavFileAudioInputReader.cpp
        WavFileAudioInputReader.h
        YinPitchDetector.cpp
        YinPitchDetector.h
        WavFilePitchesReader.cpp
        WavFilePitchesReader.h
        WavBufferReader.cpp
        WavBufferReader.h
        )
add_executable(PitchDetection ${SOURCE_FILES}
        main.cpp CppUtils/CircularBuffer.h PitchDetector.h
        AubioPitchDetector.cpp AubioPitchDetector.h)

target_link_libraries(PitchDetection ${CMAKE_SOURCE_DIR}/libs/macos/libaubio.a)

set(EXECUTABLE_OUTPUT_PATH "bin")

set(TEST_DIR test)
set(TEST_FILES tests/PitchTest.cpp tests/PitchDetectionSmoothingBufferTest.cpp CppUtils/GeometryUtils.h CppUtils/tests/GeometryUtilsTest.cpp)
add_executable(Tests ${TEST_FILES}
        ${SOURCE_FILES})