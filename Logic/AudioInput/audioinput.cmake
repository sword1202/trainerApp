set(pitchDetectionSources
        AubioPitchDetector.cpp
        AudioAverageInputLevelMonitor.cpp
        Pitch.cpp
        PitchDetectionSmoothingAudioBuffer.cpp
        PitchesRecorder.cpp
        PitchInputReader.cpp
        AudioInputPitchesRecorder.cpp
        PortAudioInputReader.cpp
        AudioInputRecorder.cpp
        )

list(TRANSFORM pitchDetectionSources PREPEND ${CMAKE_CURRENT_LIST_DIR}/)