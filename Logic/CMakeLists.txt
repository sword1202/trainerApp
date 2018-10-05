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
        Manager/ZoomController.cpp)

set(Workspace
        Workspace/PianoDrawer.cpp
        Workspace/WorkspaceDrawer.cpp)

include(Logic/AudioInput/CMakeLists.txt)
list(TRANSFORM pitchDetectionSources PREPEND AudioInput/)

include(Logic/Playback/CMakeLists.txt)
list(TRANSFORM playbackSources PREPEND Playback/)

set(logicSources
        ${Drawers} ${Manager} ${Workspace}
        ${pitchDetectionSources}
        ${playbackSources}
        nanovg/nanovg.cpp)

if (APPLE)
    list(APPEND logicSources
            nanovg/metal/nanovg_mtl.m
            nanovg/metal/nanovg_mtl_shaders.metal)
endif(APPLE)

