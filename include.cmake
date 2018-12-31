set(includes
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
        Logic/Playback/Midi
        Logic/Playback/Midi/CraigsappMidifile
        Logic
        Qt
        include/nanovg
        include/nanovg/glew)

list(TRANSFORM includes PREPEND ${CMAKE_CURRENT_LIST_DIR}/)

include_directories(${includes})

