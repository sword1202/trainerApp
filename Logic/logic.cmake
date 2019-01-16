set(Drawers
        Drawers/Drawer.cpp
        ../CppUtils/Color.cpp
        Drawers/NvgDrawer.cpp
        nanovg/nanovg.cpp
        )

if (APPLE)
    list(APPEND Drawers
            Drawers/MetalNvgDrawer.cpp
            nanovg/metal/nanovg_mtl.m
            nanovg/metal/nanovg_mtl_shaders.metal)
else()
    list(APPEND Drawers
            Drawers/OpenGLNvgDrawer.cpp)
endif(APPLE)

set(Manager
        Manager/AudioInputManager.cpp
        Manager/MainController.cpp
        Manager/PlaybackBoundsSelectionController.cpp
        )

set(Workspace
        Workspace/PianoDrawer.cpp
        Workspace/ScrollBar.cpp
        Workspace/WorkspaceDrawer.cpp)

set(logicSources
        ${Drawers}
        ${Manager}
        Events/MouseEventsReceiver.h
        Events/BaseSynchronizedMouseEventsReceiver.cpp
        ${Workspace})

list(TRANSFORM logicSources PREPEND ${CMAKE_CURRENT_LIST_DIR}/)

include(${CMAKE_CURRENT_LIST_DIR}/AudioInput/audioinput.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/Playback/playback.cmake)
list(APPEND logicSources ${playbackSources} ${pitchDetectionSources})

