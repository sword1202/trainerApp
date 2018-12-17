set(pitchDetectionSources
        AubioPitchDetector.cpp
        AudioAverageInputLevelMonitor.cpp
        AudioUtils.cpp
        Pitch.cpp
        PitchDetectionSmoothingAudioBuffer.cpp
        PitchesCollector.cpp
        PitchInputReader.cpp
        PitchInputReaderCollector.cpp
        PortAudioInputReader.cpp
        PortAudioUtils.cpp
        WAVFile.cpp
        AudioInputDataCollector.cpp
        )

list(TRANSFORM pitchDetectionSources PREPEND ${CMAKE_CURRENT_LIST_DIR}/)