cmake_minimum_required(VERSION 3.12)
project(MetalTest)

set(CMAKE_CXX_STANDARD 14)

# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

set(CMAKE_AUTORCC ON)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5QuickWidgets REQUIRED)
find_package(Qt5Svg REQUIRED)

add_executable(MetalTest main.mm
        MetalTestWidget.mm
        ../Qt/Workspace/Metal/QMetalWidget.mm
        ../Qt/Workspace/Metal/QMetalWidget.h
        ../Qt/Workspace/Metal/MetalViewCallback.h
        ../Qt/Workspace/Metal/MetalView.h
        ../Qt/Workspace/Metal/MetalView.mm
        ../Logic/nanovg/nanovg.cpp
        ../Logic/nanovg/metal/nanovg_mtl.m
        ../Logic/nanovg/metal/nanovg_mtl_shaders.metal
        ../Logic/Drawers/DrawerColor.cpp
        ../Logic/Drawers/Drawer.cpp
        ../Logic/Drawers/NvgDrawer.cpp
        ../Logic/Drawers/NvgDrawer.h)

target_link_libraries(MetalTest Qt5::Widgets)
target_link_libraries(MetalTest Qt5::QuickWidgets)
target_link_libraries(MetalTest Qt5::Svg)

include_directories(
        ../include
        ../CppUtils
        ../Logic/Drawers
        ../Qt/Workspace/Metal
        ../include/nanovg
        ../include/nanovg/glew)

target_link_libraries(MetalTest "-framework AppKit")
target_link_libraries(MetalTest "-framework Metal")
target_link_libraries(MetalTest "-framework MetalKit")
target_link_libraries(MetalTest "-framework OpenGL")
target_link_libraries(MetalTest "-framework Quartz")