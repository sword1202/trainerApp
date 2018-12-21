set(pitchDetectionSources
        AubioPitchDetector.cpp
        AudioAverageInputLevelMonitor.cpp
        Pitch.cpp
        PitchDetectionSmoothingAudioBuffer.cpp
        PitchInputReader.cpp
        AudioInputPitchesRecorder.cpp
        PortAudioInputReader.cpp
        AudioInputRecorder.cpp
        PitchesMutableList.cpp
        SeekablePitchesList.cpp
        )

list(TRANSFORM pitchDetectionSources PREPEND ${CMAKE_CURRENT_LIST_DIR}/)