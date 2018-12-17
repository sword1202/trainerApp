set(Drawers
        Drawers/Drawer.cpp
        Drawers/DrawerColor.cpp
        Drawers/NvgDrawer.cpp
        Drawers/QDrawer.cpp
        )

set(Manager
        Manager/AudioInputManager.cpp
        Manager/MainController.cpp
        Manager/PlaybackBoundsSelectionController.cpp
        Manager/WorkspaceZoomController.cpp)

set(Workspace
        Workspace/PianoDrawer.cpp
        Workspace/WorkspaceDrawer.cpp)

include(${CMAKE_CURRENT_LIST_DIR}/AudioInput/audioinput.cmake)
list(TRANSFORM pitchDetectionSources PREPEND AudioInput/)

include(${CMAKE_CURRENT_LIST_DIR}/Playback/playback.cmake)
list(TRANSFORM playbackSources PREPEND Playback/)

set(logicSources
        ${Drawers} ${Manager} ${Workspace}
        ${pitchDetectionSources}
        ${playbackSources}
        nanovg/nanovg.cpp)

if (APPLE)
    list(APPEND logicSources
            Drawers/MetalNvgDrawer.cpp
            nanovg/metal/nanovg_mtl.m
            nanovg/metal/nanovg_mtl_shaders.metal)
else()
    list(APPEND logicSources
            Drawers/OpenGLNvgDrawer.cpp)
endif(APPLE)

list(TRANSFORM logicSources PREPEND ${CMAKE_CURRENT_LIST_DIR}/)

