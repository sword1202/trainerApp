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

include(../Logic/CMakeLists.txt)
list(TRANSFORM logicSources PREPEND ../Logic/)

set(cppUtilsSources
        ../CppUtils/TimeUtils.cpp)

set(sources ${logicSources} ${cppUtilsSources})

find_package(Qt5Widgets REQUIRED)
find_package(Qt5QuickWidgets REQUIRED)
find_package(Qt5Svg REQUIRED)

add_executable(MvxGenerator ${sources} main.cpp)